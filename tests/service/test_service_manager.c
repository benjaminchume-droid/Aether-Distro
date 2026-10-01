#include <assert.h>
#include <unistd.h>
#include <sys/wait.h>
#include "aether/service_manager.h"
static aether_status_t ok(void *x){(void)x;return AETHER_OK;}
int main(void){
 assert(aether_service_manager_init()==AETHER_OK);
 char *argv[]={(char*)"/bin/true",NULL};
 aether_service_runtime_t s={.id=1,.name="test",.exec_path="/bin/true",.argv=argv,.restart_on_failure=0};
 assert(aether_service_register(&s)==AETHER_OK);
 assert(aether_service_count()==1);
 assert(aether_service_start(&s)==AETHER_OK);
 int status; pid_t p=waitpid(s.pid,&status,0); assert(p>0);
 assert(aether_service_reap(p,status)==AETHER_OK);
 assert(s.state==AETHER_SERVICE_STOPPED);
 return 0;
}
