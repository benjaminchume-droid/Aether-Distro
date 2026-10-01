#define _GNU_SOURCE
#include <aether/init.h>
#include <aether/service_manager.h>
#include <aether/capability_registry.h>
#include <aether/device_manager.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/reboot.h>
#include <sys/wait.h>
#include <unistd.h>

static volatile sig_atomic_t shutting_down;
static void log_line(const char *s){int fd=open("/dev/console",O_WRONLY|O_NOCTTY);if(fd>=0){const char *p=s;while(*p)++p;write(fd,s,(size_t)(p-s));close(fd);}}
static void signal_handler(int sig){if(sig==SIGTERM||sig==SIGINT||sig==SIGQUIT)shutting_down=1;}
static void reap_children(void){int status;pid_t pid;while((pid=waitpid(-1,&status,WNOHANG))>0)aether_service_reap(pid,status);}
static int boot_step(aether_status_t s,const char *ok,const char *fail){if(s!=AETHER_OK){log_line(fail);return 1;}log_line(ok);return 0;}
int main(void){
 if(getpid()!=1)return 1;
 log_line("Aether PID 1: starting\n");
 if(boot_step(aether_init_early(),"Aether: early init OK\n","Aether: early init FAILED\n"))return 1;
 struct sigaction sa={0};sa.sa_handler=signal_handler;sigemptyset(&sa.sa_mask);
 sigaction(SIGTERM,&sa,0);sigaction(SIGINT,&sa,0);sigaction(SIGQUIT,&sa,0);
 if(boot_step(aether_init_mounts(),"Aether: early mounts OK\n","Aether: early mounts FAILED\n"))return 1;
 if(boot_step(aether_capability_registry_init(),"Aether: capability registry OK\n","Aether: capability registry FAILED\n"))return 1;
 if(boot_step(aether_device_manager_init(),"Aether: device manager OK\n","Aether: device manager FAILED\n"))return 1;
 if(boot_step(aether_device_scan(),"Aether: device scan OK\n","Aether: device scan FAILED\n"))return 1;
 if(boot_step(aether_service_manager_init(),"Aether: service manager OK\n","Aether: service manager FAILED\n"))return 1;
 if(boot_step(aether_init_services(),"Aether: services OK\n","Aether: services FAILED\n"))return 1;
 log_line("Aether: boot complete; entering supervisor loop\n");
 while(!shutting_down){reap_children();sleep(1);}
 log_line("Aether: shutting down\n");
 aether_service_manager_shutdown();aether_init_shutdown();
 reboot(RB_POWER_OFF);
 return 0;
}
