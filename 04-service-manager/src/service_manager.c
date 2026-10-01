#include <aether/service_manager.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define AETHER_MAX_SERVICES 64
static aether_service_runtime_t *services[AETHER_MAX_SERVICES];
static size_t service_count;

static aether_service_runtime_t *find_name(const char *name) {
    for (size_t i=0;i<service_count;i++)
        if (services[i] && services[i]->name && strcmp(services[i]->name,name)==0) return services[i];
    return NULL;
}
static aether_status_t dependencies_ready(const aether_service_runtime_t *s) {
    for (size_t i=0;i<s->dependency_count;i++) {
        aether_service_runtime_t *d=find_name(s->dependencies[i]);
        if (!d) return AETHER_ERR_NOT_FOUND;
        if (d->state!=AETHER_SERVICE_RUNNING) return AETHER_ERR_BUSY;
    }
    return AETHER_OK;
}
aether_status_t aether_service_manager_init(void){service_count=0;return AETHER_OK;}
aether_status_t aether_service_register(aether_service_runtime_t *s){
    if(!s||!s->name||!s->exec_path||!s->argv) return AETHER_ERR_INVALID;
    if(service_count>=AETHER_MAX_SERVICES) return AETHER_ERR_LIMIT;
    if(find_name(s->name)) return AETHER_ERR_EXISTS;
    s->state=AETHER_SERVICE_STOPPED; s->pid=0; services[service_count++]=s; return AETHER_OK;
}
aether_status_t aether_service_start(aether_service_runtime_t *s){
    if(!s||!s->name||!s->exec_path||!s->argv)return AETHER_ERR_INVALID;
    if(s->state==AETHER_SERVICE_RUNNING)return AETHER_ERR_BUSY;
    aether_status_t dep=dependencies_ready(s); if(dep!=AETHER_OK)return dep;
    s->state=AETHER_SERVICE_STARTING;
    pid_t pid=fork();
    if(pid<0){s->state=AETHER_SERVICE_FAILED;return AETHER_ERR_IO;}
    if(pid==0){setsid();execv(s->exec_path,s->argv);_exit(127);}
    s->pid=pid; s->state=AETHER_SERVICE_RUNNING; return AETHER_OK;
}
aether_status_t aether_service_stop(aether_service_runtime_t *s){
    if(!s||!s->name)return AETHER_ERR_INVALID;
    if(s->state==AETHER_SERVICE_STOPPED)return AETHER_OK;
    if(s->pid>0 && kill(s->pid,SIGTERM)<0 && errno!=ESRCH)return AETHER_ERR_IO;
    s->state=AETHER_SERVICE_STOPPED; s->pid=0; return AETHER_OK;
}
aether_status_t aether_service_reap(pid_t pid,int status){
    for(size_t i=0;i<service_count;i++) if(services[i]&&services[i]->pid==pid){
        services[i]->pid=0;
        services[i]->state=WIFEXITED(status)&&WEXITSTATUS(status)==0?AETHER_SERVICE_STOPPED:AETHER_SERVICE_FAILED;
        return AETHER_OK;
    }
    return AETHER_ERR_NOT_FOUND;
}
aether_status_t aether_service_manager_shutdown(void){
    for(size_t i=service_count;i>0;i--) if(services[i-1]) aether_service_stop(services[i-1]);
    service_count=0; return AETHER_OK;
}
size_t aether_service_count(void){return service_count;}
