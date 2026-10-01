#define _GNU_SOURCE
#include "aether/sandbox.h"
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <linux/filter.h>
#include <linux/audit.h>
#include <linux/unistd.h>
#include <linux/landlock.h>
#include <linux/seccomp.h>
#include <sched.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/mount.h>
#include <stdlib.h>
#include <sys/prctl.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/wait.h>
#include <unistd.h>

static int set_limit(int resource,uint64_t value){
    if(!value) return 0;
    struct rlimit limit={(rlim_t)value,(rlim_t)value};
    return setrlimit(resource,&limit)==0?0:-1;
}

static int write_text_file(const char *path,const char *text){
    int fd=open(path,O_WRONLY|O_CLOEXEC);
    if(fd<0) return -1;
    size_t len=strlen(text);
    ssize_t written=write(fd,text,len);
    int saved=errno;
    close(fd);
    errno=saved;
    return written==(ssize_t)len?0:-1;
}

static int apply_user_namespace(void){
    uid_t uid=getuid();
    gid_t gid=getgid();
    char mapping[128];

    if(unshare(CLONE_NEWUSER)!=0) return -1;
    if(write_text_file("/proc/self/setgroups","deny")!=0 && errno!=ENOENT) return -1;

    int n=snprintf(mapping,sizeof(mapping),"0 %u 1\n",(unsigned)uid);
    if(n<=0 || (size_t)n>=sizeof(mapping) || write_text_file("/proc/self/uid_map",mapping)!=0) return -1;
    n=snprintf(mapping,sizeof(mapping),"0 %u 1\n",(unsigned)gid);
    if(n<=0 || (size_t)n>=sizeof(mapping) || write_text_file("/proc/self/gid_map",mapping)!=0) return -1;
    return 0;
}

static int apply_namespaces(uint32_t flags){
    int ns=0;
    if(flags&AETHER_SANDBOX_NEW_MOUNT_NS) ns|=CLONE_NEWNS;
    if(flags&AETHER_SANDBOX_NEW_PID_NS) ns|=CLONE_NEWPID;
    if(flags&AETHER_SANDBOX_NEW_NET_NS) ns|=CLONE_NEWNET;
    if(flags&AETHER_SANDBOX_NEW_IPC_NS) ns|=CLONE_NEWIPC;
    if(flags&AETHER_SANDBOX_NEW_UTS_NS) ns|=CLONE_NEWUTS;
    if(flags&AETHER_SANDBOX_NEW_USER_NS){
        if(apply_user_namespace()!=0) return -1;
    }
    if(ns && unshare(ns)!=0) return -1;
    if(flags&AETHER_SANDBOX_PRIVATE_MOUNTS){
        if(mount(NULL,"/",NULL,MS_REC|MS_PRIVATE,NULL)!=0) return -1;
    }
    return 0;
}

static int apply_limits(const aether_sandbox_profile_t *p){
    if(set_limit(RLIMIT_AS,p->memory_bytes)!=0) return -1;
    if(set_limit(RLIMIT_CPU,p->cpu_seconds)!=0) return -1;
    if(set_limit(RLIMIT_FSIZE,p->file_bytes)!=0) return -1;
    if(set_limit(RLIMIT_NPROC,p->process_count)!=0) return -1;
    return 0;
}

static int landlock_available(void){
#ifdef SYS_landlock_create_ruleset
    int rc=(int)syscall(SYS_landlock_create_ruleset,NULL,0,LANDLOCK_CREATE_RULESET_VERSION);
    return rc>=0;
#else
    return 0;
#endif
}

static int apply_landlock(const aether_sandbox_profile_t *p){
#ifdef SYS_landlock_create_ruleset
    if(!p->readonly_path_count) return 0;
    if(!landlock_available()) { errno=ENOSYS; return -1; }

    struct landlock_ruleset_attr ruleset={0};
    ruleset.handled_access_fs=
        LANDLOCK_ACCESS_FS_EXECUTE|LANDLOCK_ACCESS_FS_WRITE_FILE|
        LANDLOCK_ACCESS_FS_READ_FILE|LANDLOCK_ACCESS_FS_READ_DIR|
        LANDLOCK_ACCESS_FS_REMOVE_DIR|LANDLOCK_ACCESS_FS_REMOVE_FILE|
        LANDLOCK_ACCESS_FS_MAKE_CHAR|LANDLOCK_ACCESS_FS_MAKE_DIR|
        LANDLOCK_ACCESS_FS_MAKE_REG|LANDLOCK_ACCESS_FS_MAKE_SOCK|
        LANDLOCK_ACCESS_FS_MAKE_FIFO|LANDLOCK_ACCESS_FS_MAKE_BLOCK|
        LANDLOCK_ACCESS_FS_MAKE_SYM;

    int ruleset_fd=(int)syscall(SYS_landlock_create_ruleset,&ruleset,sizeof(ruleset),0);
    if(ruleset_fd<0) return -1;

    for(size_t i=0;i<p->readonly_path_count;i++){
        if(!p->readonly_paths[i] || !*p->readonly_paths[i]) continue;
        int fd=open(p->readonly_paths[i],O_PATH|O_CLOEXEC);
        if(fd<0){int saved=errno;close(ruleset_fd);errno=saved;return -1;}
        struct landlock_path_beneath_attr rule={
            .parent_fd=fd,
            .allowed_access=LANDLOCK_ACCESS_FS_EXECUTE|LANDLOCK_ACCESS_FS_READ_FILE|
                             LANDLOCK_ACCESS_FS_READ_DIR
        };
        int rc=(int)syscall(SYS_landlock_add_rule,ruleset_fd,LANDLOCK_RULE_PATH_BENEATH,&rule,0);
        int saved=errno;
        close(fd);
        if(rc<0){close(ruleset_fd);errno=saved;return -1;}
    }

    if(prctl(PR_SET_NO_NEW_PRIVS,1,0,0,0)!=0){int saved=errno;close(ruleset_fd);errno=saved;return -1;}
    int rc=(int)syscall(SYS_landlock_restrict_self,ruleset_fd,0);
    int saved=errno;
    close(ruleset_fd);
    errno=saved;
    return rc;
#else
    (void)p;
    errno=ENOSYS;
    return -1;
#endif
}

static int cgroupv2_mountpoint(char *out,size_t size){
    FILE *f=fopen("/proc/self/mountinfo","r");
    if(!f) return -1;
    char line[4096];
    while(fgets(line,sizeof(line),f)){
        char *dash=strstr(line," - cgroup2 ");
        if(!dash) continue;
        char mountpoint[256]={0};
        if(sscanf(line,"%*s %*s %*s %*s %255s",mountpoint)==1){
            snprintf(out,size,"%s",mountpoint);
            fclose(f);
            return 0;
        }
    }
    fclose(f);
    return -1;
}

static int cgroupv2_available(void){
    char mountpoint[256];
    return cgroupv2_mountpoint(mountpoint,sizeof(mountpoint))==0;
}

static int write_limit(const char *dir,const char *name,uint64_t value){
    char path[512];
    snprintf(path,sizeof(path),"%s/%s",dir,name);
    char value_text[64];
    snprintf(value_text,sizeof(value_text),"%llu",(unsigned long long)value);
    return write_text_file(path,value_text);
}

static int seccomp_audit_arch(void){
#if defined(__x86_64__)
    return AUDIT_ARCH_X86_64;
#elif defined(__aarch64__)
    return AUDIT_ARCH_AARCH64;
#else
    return -1;
#endif
}

static int apply_seccomp_allowlist(const aether_sandbox_profile_t *p){
#ifdef PR_SET_SECCOMP
    if(!p->seccomp_syscall_count) return 0;
    if(!p->seccomp_syscalls) { errno=EINVAL; return -1; }
    if(seccomp_audit_arch()<0){ errno=ENOTSUP; return -1; }

    size_t count=p->seccomp_syscall_count;
    struct sock_filter *filter=calloc(count*2+5,sizeof(*filter));
    if(!filter){errno=ENOMEM;return -1;}

    size_t n=0;
    filter[n++]=(struct sock_filter)BPF_STMT(BPF_LD|BPF_W|BPF_ABS,offsetof(struct seccomp_data,arch));
    filter[n++]=(struct sock_filter)BPF_JUMP(BPF_JMP|BPF_JEQ|BPF_K,(uint32_t)seccomp_audit_arch(),1,0);
    filter[n++]=(struct sock_filter)BPF_STMT(BPF_RET|BPF_K,SECCOMP_RET_KILL_PROCESS);
    filter[n++]=(struct sock_filter)BPF_STMT(BPF_LD|BPF_W|BPF_ABS,offsetof(struct seccomp_data,nr));

    for(size_t i=0;i<count;i++){
        if(p->seccomp_syscalls[i]<0){
            free(filter);
            errno=EINVAL;
            return -1;
        }
        filter[n++]=(struct sock_filter)BPF_JUMP(BPF_JMP|BPF_JEQ|BPF_K,(uint32_t)p->seccomp_syscalls[i],0,1);
        filter[n++]=(struct sock_filter)BPF_STMT(BPF_RET|BPF_K,SECCOMP_RET_ALLOW);
    }
    filter[n++]=(struct sock_filter)BPF_STMT(BPF_RET|BPF_K,SECCOMP_RET_KILL_PROCESS);

    struct sock_fprog program={(unsigned short)n,filter};
    if(prctl(PR_SET_NO_NEW_PRIVS,1,0,0,0)!=0){int saved=errno;free(filter);errno=saved;return -1;}
    int rc=prctl(PR_SET_SECCOMP,SECCOMP_MODE_FILTER,&program,0,0);
    int saved=errno;
    free(filter);
    errno=saved;
    return rc;
#else
    (void)p;
    errno=ENOSYS;
    return -1;
#endif
}

static int apply_cgroup(const aether_sandbox_profile_t *p){
    if(!p->cgroup_name || !*p->cgroup_name) return 0;
    char mountpoint[256];
    if(cgroupv2_mountpoint(mountpoint,sizeof(mountpoint))!=0){errno=ENOSYS;return -1;}
    if(strlen(p->cgroup_name)>128 || strchr(p->cgroup_name,'/') || strcmp(p->cgroup_name,".")==0 || strcmp(p->cgroup_name,"..")==0){errno=EINVAL;return -1;}

    char dir[512];
    snprintf(dir,sizeof(dir),"%s/%s",mountpoint,p->cgroup_name);
    if(mkdir(dir,0755)!=0 && errno!=EEXIST) return -1;

    if(p->memory_bytes && write_limit(dir,"memory.max",p->memory_bytes)!=0) return -1;
    if(p->cpu_percent){
        if(p->cpu_percent>100){errno=EINVAL;return -1;}
        char path[512],value[64];
        snprintf(path,sizeof(path),"%s/cpu.max",dir);
        unsigned long long quota=(unsigned long long)p->cpu_percent*10000ULL;
        snprintf(value,sizeof(value),"%llu 1000000",quota);
        if(write_text_file(path,value)!=0) return -1;
    }
    if(p->process_count && write_limit(dir,"pids.max",p->process_count)!=0) return -1;

    char procs[512];
    snprintf(procs,sizeof(procs),"%s/cgroup.procs",dir);
    char pid_text[32];
    snprintf(pid_text,sizeof(pid_text),"%ld",(long)getpid());
    return write_text_file(procs,pid_text);
}

int aether_sandbox_linux_available(void){
#ifdef __linux__
    return 1;
#else
    return 0;
#endif
}

int aether_sandbox_landlock_available(void){return landlock_available();}
int aether_sandbox_cgroupv2_available(void){return cgroupv2_available();}

aether_status_t aether_sandbox_apply(const aether_sandbox_profile_t *profile){
    if(!profile) return AETHER_ERR_INVALID;
    if(!aether_sandbox_linux_available()) return AETHER_ERR_UNAVAILABLE;
    if(apply_namespaces(profile->flags)!=0) return (errno==EPERM || errno==EACCES)?AETHER_ERR_PERMISSION:AETHER_ERR_IO;
    if(profile->flags&AETHER_SANDBOX_NO_NEW_PRIVS){
        if(prctl(PR_SET_NO_NEW_PRIVS,1,0,0,0)!=0) return AETHER_ERR_PERMISSION;
    }
    if(profile->flags&AETHER_SANDBOX_STRICT_SECCOMP){
        if(!(profile->flags&AETHER_SANDBOX_NO_NEW_PRIVS) && prctl(PR_SET_NO_NEW_PRIVS,1,0,0,0)!=0) return AETHER_ERR_PERMISSION;
        if(prctl(PR_SET_SECCOMP,SECCOMP_MODE_STRICT,0,0,0)!=0) return AETHER_ERR_PERMISSION;
    }
    if(profile->flags&AETHER_SANDBOX_SECCOMP_ALLOWLIST){
        if(apply_seccomp_allowlist(profile)!=0)
            return (errno==EPERM || errno==EACCES)?AETHER_ERR_PERMISSION:
                   (errno==ENOSYS || errno==ENOTSUP)?AETHER_ERR_UNAVAILABLE:AETHER_ERR_INVALID;
    }
    if((profile->flags&AETHER_SANDBOX_LANDLOCK_RO) && apply_landlock(profile)!=0)
        return (errno==EPERM || errno==EACCES)?AETHER_ERR_PERMISSION:AETHER_ERR_UNAVAILABLE;
    if((profile->flags&AETHER_SANDBOX_CGROUP_LIMITS) && apply_cgroup(profile)!=0)
        return (errno==EPERM || errno==EACCES)?AETHER_ERR_PERMISSION:AETHER_ERR_IO;
    if(apply_limits(profile)!=0) return AETHER_ERR_IO;
    return AETHER_OK;
}

aether_status_t aether_sandbox_spawn(const aether_sandbox_profile_t *profile,
                                     const char *path,char *const argv[],char *const envp[],
                                     pid_t *pid_out){
    if(!profile || !path || !*path || !argv || !argv[0] || !pid_out) return AETHER_ERR_INVALID;
    pid_t pid=fork();
    if(pid<0) return AETHER_ERR_IO;
    if(pid==0){
        if(aether_sandbox_apply(profile)!=AETHER_OK) _exit(126);
        if(profile->flags&AETHER_SANDBOX_NEW_PID_NS){
            pid_t init_child=fork();
            if(init_child<0) _exit(126);
            if(init_child>0){
                int status=0;
                if(waitpid(init_child,&status,0)<0) _exit(126);
                if(WIFEXITED(status)) _exit(WEXITSTATUS(status));
                if(WIFSIGNALED(status)) _exit(128+WTERMSIG(status));
                _exit(126);
            }
        }
        if(envp) execve(path,argv,envp); else execv(path,argv);
        _exit(127);
    }
    *pid_out=pid;
    return AETHER_OK;
}

aether_status_t aether_sandbox_spawn_subject(aether_id_t subject,const aether_sandbox_profile_t *profile,
                                     const char *path,char *const argv[],char *const envp[],
                                     pid_t *pid_out){
    if(!subject || !profile) return AETHER_ERR_INVALID;

    aether_aegis_request_t process_request={subject,AETHER_AEGIS_RESOURCE_PROCESS,path};
    if(aether_aegis_evaluate(&process_request)!=AETHER_AEGIS_DECISION_ALLOW)
        return AETHER_ERR_PERMISSION;

    aether_sandbox_profile_t effective=*profile;

    aether_aegis_request_t network_request={subject,AETHER_AEGIS_RESOURCE_NETWORK,NULL};
    if(aether_aegis_has_rule(subject,AETHER_AEGIS_RESOURCE_NETWORK,NULL) &&
       aether_aegis_evaluate(&network_request)==AETHER_AEGIS_DECISION_DENY)
        effective.flags|=AETHER_SANDBOX_NEW_NET_NS;

    aether_aegis_request_t ipc_request={subject,AETHER_AEGIS_RESOURCE_IPC,NULL};
    if(aether_aegis_has_rule(subject,AETHER_AEGIS_RESOURCE_IPC,NULL) &&
       aether_aegis_evaluate(&ipc_request)==AETHER_AEGIS_DECISION_DENY)
        effective.flags|=AETHER_SANDBOX_NEW_IPC_NS;

    return aether_sandbox_spawn(&effective,path,argv,envp,pid_out);
}
