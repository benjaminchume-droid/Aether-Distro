#include <assert.h>
#include <linux/unistd.h>
#include <sys/wait.h>
#include <unistd.h>
#include "aether/sandbox.h"
#include "aether/aegis.h"

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
    pid=fork();
    assert(pid>=0);
    if(pid==0){
        assert(aether_sandbox_apply(&profile)==AETHER_OK);
        _exit(0);
    }
    assert(waitpid(pid,&status,0)==pid);
    assert(WIFEXITED(status) && WEXITSTATUS(status)==0);

    aether_aegis_shutdown();
    return 0;
}
