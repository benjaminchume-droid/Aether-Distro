#include <assert.h>
#include <sys/wait.h>
#include <unistd.h>
#include "aether/service_manager.h"

static void wait_reap(aether_service_runtime_t *s){
    int status;
    pid_t p=waitpid(s->pid,&status,0);
    assert(p>0);
    assert(aether_service_reap(p,status)==AETHER_OK);
}
int main(void){
    assert(aether_service_manager_init()==AETHER_OK);
    char *true_argv[]={(char*)"/bin/true",0};
    aether_service_runtime_t base={.id=1,.name="base",.exec_path="/bin/true",.argv=true_argv};
    aether_service_runtime_t dep={.id=2,.name="dependent",.exec_path="/bin/true",.argv=true_argv,.dependencies=(const char*[]){"base"},.dependency_count=1};
    assert(aether_service_register(&base)==AETHER_OK);
    assert(aether_service_register(&dep)==AETHER_OK);
    assert(aether_service_start(&dep)==AETHER_ERR_BUSY);
    assert(aether_service_start(&base)==AETHER_OK);
    wait_reap(&base);
    assert(aether_service_start(&dep)==AETHER_OK);
    wait_reap(&dep);
    assert(aether_service_count()==2);
    assert(aether_service_manager_shutdown()==AETHER_OK);
    return 0;
}
