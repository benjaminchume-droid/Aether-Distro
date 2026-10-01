#define _GNU_SOURCE
#include "aether/sandbox.h"
#include <errno.h>
#include <fcntl.h>
#include <linux/filter.h>
#include <linux/seccomp.h>
#include <sched.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mount.h>
#include <sys/prctl.h>
#include <sys/resource.h>
#include <sys/syscall.h>
#include <sys/wait.h>
#include <unistd.h>

static int set_limit(int resource, uint64_t value){
    if(!value) return 0;
    struct rlimit limit;
    limit.rlim_cur=(rlim_t)value;
    limit.rlim_max=(rlim_t)value;
    return setrlimit(resource,&limit)==0 ? 0 : -1;
}

static int apply_namespaces(uint32_t flags){
    int ns=0;
    if(flags&AETHER_SANDBOX_NEW_USER_NS) ns|=CLONE_NEWUSER;
    if(flags&AETHER_SANDBOX_NEW_MOUNT_NS) ns|=CLONE_NEWNS;
    if(flags&AETHER_SANDBOX_NEW_PID_NS) ns|=CLONE_NEWPID;
    if(flags&AETHER_SANDBOX_NEW_NET_NS) ns|=CLONE_NEWNET;
    if(flags&AETHER_SANDBOX_NEW_IPC_NS) ns|=CLONE_NEWIPC;
    if(flags&AETHER_SANDBOX_NEW_UTS_NS) ns|=CLONE_NEWUTS;
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

int aether_sandbox_linux_available(void){
#ifdef __linux__
    return 1;
#else
    return 0;
#endif
}

aether_status_t aether_sandbox_apply(const aether_sandbox_profile_t *profile){
    if(!profile) return AETHER_ERR_INVALID;
    if(!aether_sandbox_linux_available()) return AETHER_ERR_UNAVAILABLE;
    if(apply_namespaces(profile->flags)!=0) return (errno==EPERM || errno==EACCES)?AETHER_ERR_PERMISSION:AETHER_ERR_IO;
    if(profile->flags&AETHER_SANDBOX_NO_NEW_PRIVS){
        if(prctl(PR_SET_NO_NEW_PRIVS,1,0,0,0)!=0) return AETHER_ERR_PERMISSION;
    }
    if(profile->flags&AETHER_SANDBOX_STRICT_SECCOMP){
        if(!(profile->flags&AETHER_SANDBOX_NO_NEW_PRIVS) &&
           prctl(PR_SET_NO_NEW_PRIVS,1,0,0,0)!=0) return AETHER_ERR_PERMISSION;
        if(prctl(PR_SET_SECCOMP,SECCOMP_MODE_STRICT,0,0,0)!=0) return AETHER_ERR_PERMISSION;
    }
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
        if(envp) execve(path,argv,envp); else execv(path,argv);
        _exit(127);
    }
    *pid_out=pid;
    return AETHER_OK;
}
