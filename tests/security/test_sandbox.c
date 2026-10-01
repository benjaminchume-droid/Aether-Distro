#define _GNU_SOURCE
#include <assert.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "aether/sandbox.h"
#include "aether/aegis.h"

static void check_landlock(void){
    if(!aether_sandbox_landlock_available()) return;

    char dir_template[]="/tmp/aether-landlock-XXXXXX";
    char *dir=mkdtemp(dir_template);
    assert(dir!=NULL);

    const char *readonly[]={dir};
    aether_sandbox_profile_t profile={
        .flags=AETHER_SANDBOX_LANDLOCK_RO|AETHER_SANDBOX_NO_NEW_PRIVS,
        .readonly_paths=readonly,
        .readonly_path_count=1
    };

    pid_t pid=fork();
    assert(pid>=0);
    if(pid==0){
        if(aether_sandbox_apply(&profile)!=AETHER_OK) _exit(2);
        char path[512];
        snprintf(path,sizeof(path),"%s/blocked",dir);
        int fd=open(path,O_WRONLY|O_CREAT|O_TRUNC,0600);
        if(fd<0) _exit(0);
        close(fd);
        _exit(1);
    }
    int status=0;
    assert(waitpid(pid,&status,0)==pid);
    assert(WIFEXITED(status) && WEXITSTATUS(status)==0);

    char allowed_path[512];
    snprintf(allowed_path,sizeof(allowed_path),"%s/allowed",dir);
    const char *writable[]={dir};
    profile=(aether_sandbox_profile_t){
        .flags=AETHER_SANDBOX_LANDLOCK_RO|AETHER_SANDBOX_NO_NEW_PRIVS,
        .writable_paths=writable,
        .writable_path_count=1
    };

    pid=fork();
    assert(pid>=0);
    if(pid==0){
        if(aether_sandbox_apply(&profile)!=AETHER_OK) _exit(2);
        int fd=open(allowed_path,O_WRONLY|O_CREAT|O_TRUNC,0600);
        if(fd<0) _exit(1);
        if(write(fd,"ok",2)!=2){close(fd);_exit(1);}
        close(fd);
        _exit(0);
    }
    assert(waitpid(pid,&status,0)==pid);
    assert(WIFEXITED(status) && WEXITSTATUS(status)==0);
    unlink(allowed_path);
    rmdir(dir);
}

int main(void){
    assert(aether_aegis_init()==AETHER_OK);
    assert(aether_sandbox_linux_available()==1);

    aether_sandbox_profile_t profile={.flags=AETHER_SANDBOX_NO_NEW_PRIVS};
    pid_t pid=0;
    char *argv[]={"/bin/true",NULL};
    assert(aether_sandbox_spawn_subject(42,&profile,"/bin/true",argv,NULL,&pid)==AETHER_ERR_PERMISSION);

    aether_aegis_rule_t rule={42,AETHER_AEGIS_RESOURCE_PROCESS,AETHER_AEGIS_DECISION_ALLOW,"/bin/true",10};
    assert(aether_aegis_add_rule(&rule)==AETHER_OK);
    assert(aether_sandbox_spawn_subject(42,&profile,"/bin/true",argv,NULL,&pid)==AETHER_OK);
    int status=0;
    assert(waitpid(pid,&status,0)==pid);
    assert(WIFEXITED(status) && WEXITSTATUS(status)==0);

    int allowed[]={__NR_exit_group};
    profile=(aether_sandbox_profile_t){
        .flags=AETHER_SANDBOX_NO_NEW_PRIVS|AETHER_SANDBOX_SECCOMP_ALLOWLIST,
        .seccomp_syscalls=allowed,
        .seccomp_syscall_count=1
    };
    if(aether_sandbox_seccomp_available()){
        pid=fork();
        assert(pid>=0);
        if(pid==0){
            aether_status_t sandbox_status=aether_sandbox_apply(&profile);
            if(sandbox_status!=AETHER_OK){
                dprintf(STDERR_FILENO,"seccomp apply failed: status=%d errno=%d\\n",(int)sandbox_status,errno);
                _exit(100+(int)sandbox_status);
            }
            _exit(0);
        }
        assert(waitpid(pid,&status,0)==pid);
        assert(WIFEXITED(status) && WEXITSTATUS(status)==0);
    }

    check_landlock();

    aether_aegis_shutdown();
    return 0;
}
