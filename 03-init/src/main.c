#include <aether/init.h>
#include <aether/service_manager.h>
#include <aether/capability_registry.h>
#include <aether/device_manager.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

static volatile sig_atomic_t shutting_down;
static void signal_handler(int sig){if(sig==SIGTERM||sig==SIGINT||sig==SIGQUIT)shutting_down=1;}
static void reap_children(void){
    int status; pid_t pid;
    while((pid=waitpid(-1,&status,WNOHANG))>0) aether_service_reap(pid,status);
}
int main(void){
    if(aether_init_early()!=AETHER_OK)return 1;
    struct sigaction sa={0}; sa.sa_handler=signal_handler; sigemptyset(&sa.sa_mask);
    sigaction(SIGTERM,&sa,NULL); sigaction(SIGINT,&sa,NULL); sigaction(SIGQUIT,&sa,NULL);
    signal(SIGCHLD,SIG_DFL);
    if(aether_init_mounts()!=AETHER_OK)return 1;
    if(aether_capability_registry_init()!=AETHER_OK)return 1;
    if(aether_device_manager_init()!=AETHER_OK)return 1;
    if(aether_device_scan()!=AETHER_OK)return 1;
    if(aether_service_manager_init()!=AETHER_OK)return 1;
    if(aether_init_services()!=AETHER_OK)return 1;
    while(!shutting_down){reap_children();sleep(1);}
    aether_service_manager_shutdown(); aether_init_shutdown(); return 0;
}
