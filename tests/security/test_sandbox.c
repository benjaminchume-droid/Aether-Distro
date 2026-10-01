#include <assert.h>
#include <sys/wait.h>
#include <unistd.h>
#include "aether/sandbox.h"
#include "aether/aegis.h"

int main(void){
 assert(aether_aegis_init()==AETHER_OK);
 assert(aether_sandbox_linux_available()==1);

 aether_sandbox_profile_t profile={AETHER_SANDBOX_NO_NEW_PRIVS,0,0,0,0};
 pid_t pid=0;
 char *argv[]={"/bin/true",NULL};
 assert(aether_sandbox_spawn_subject(42,&profile,"/bin/true",argv,NULL,&pid)==AETHER_ERR_PERMISSION);

 aether_aegis_rule_t rule={42,AETHER_AEGIS_RESOURCE_PROCESS,AETHER_AEGIS_DECISION_ALLOW,"/bin/true",10};
 assert(aether_aegis_add_rule(&rule)==AETHER_OK);
 assert(aether_sandbox_spawn_subject(42,&profile,"/bin/true",argv,NULL,&pid)==AETHER_OK);
 int status=0;
 assert(waitpid(pid,&status,0)==pid);
 assert(WIFEXITED(status) && WEXITSTATUS(status)==0);

 aether_aegis_shutdown();
 return 0;
}
