#define _GNU_SOURCE
#include <aether/service_manager.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define AETHER_MAX_SERVICES 64
static aether_service_runtime_t *services[AETHER_MAX_SERVICES];
static size_t service_count;

static aether_service_runtime_t *find_name(const char *name){
    if(!name) return NULL;
    for(size_t i=0;i<service_count;i++)
        if(services[i] && services[i]->name && strcmp(services[i]->name,name)==0) return services[i];
    return NULL;
}

static aether_status_t dependencies_ready(const aether_service_runtime_t *s){
    if(!s) return AETHER_ERR_INVALID;
    for(size_t i=0;i<s->dependency_count;i++){
        if(!s->dependencies || !s->dependencies[i]) return AETHER_ERR_INVALID;
        aether_service_runtime_t *d=find_name(s->dependencies[i]);
        if(!d) return AETHER_ERR_NOT_FOUND;
        if(d->state!=AETHER_SERVICE_RUNNING) return AETHER_ERR_BUSY;
    }
    return AETHER_OK;
}

aether_status_t aether_service_manager_init(void){
    service_count=0;
    memset(services,0,sizeof(services));
    return AETHER_OK;
}

aether_status_t aether_service_register(aether_service_runtime_t *s){
    if(!s || !s->name || !*s->name || !s->exec_path || !*s->exec_path || !s->argv)
        return AETHER_ERR_INVALID;
    if(service_count>=AETHER_MAX_SERVICES) return AETHER_ERR_LIMIT;
    if(find_name(s->name)) return AETHER_ERR_EXISTS;
    s->state=AETHER_SERVICE_STOPPED;
    s->pid=0;
    services[service_count++]=s;
    return AETHER_OK;
}

aether_status_t aether_service_start(aether_service_runtime_t *s){
    if(!s || !s->name || !s->exec_path || !s->argv) return AETHER_ERR_INVALID;
    if(s->state==AETHER_SERVICE_RUNNING || s->state==AETHER_SERVICE_STARTING ||
       s->state==AETHER_SERVICE_STOPPING) return AETHER_ERR_BUSY;
    aether_status_t dep=dependencies_ready(s);
    if(dep!=AETHER_OK) return dep;

    s->state=AETHER_SERVICE_STARTING;
    pid_t pid=fork();
    if(pid<0){s->state=AETHER_SERVICE_FAILED;return AETHER_ERR_IO;}
    if(pid==0){
        if(setsid()<0) _exit(126);
        execv(s->exec_path,s->argv);
        _exit(127);
    }
    s->pid=pid;
    s->state=AETHER_SERVICE_RUNNING;
    return AETHER_OK;
}

aether_status_t aether_service_stop(aether_service_runtime_t *s){
    if(!s || !s->name) return AETHER_ERR_INVALID;
    if(s->state==AETHER_SERVICE_STOPPED && s->pid<=0) return AETHER_OK;
    if(s->state==AETHER_SERVICE_STOPPING) return AETHER_ERR_BUSY;
    if(s->pid>0){
        if(kill(s->pid,SIGTERM)<0 && errno!=ESRCH) return AETHER_ERR_IO;
        s->state=AETHER_SERVICE_STOPPING;
        return AETHER_OK;
    }
    s->state=AETHER_SERVICE_STOPPED;
    return AETHER_OK;
}

aether_status_t aether_service_reap(pid_t pid,int status){
    if(pid<=0) return AETHER_ERR_INVALID;
    for(size_t i=0;i<service_count;i++) if(services[i] && services[i]->pid==pid){
        aether_service_runtime_t *s=services[i];
        int stopping=(s->state==AETHER_SERVICE_STOPPING);
        s->pid=0;
        s->state=stopping ? AETHER_SERVICE_STOPPED :
            (WIFEXITED(status) && WEXITSTATUS(status)==0 ? AETHER_SERVICE_STOPPED : AETHER_SERVICE_FAILED);
        return AETHER_OK;
    }
    return AETHER_ERR_NOT_FOUND;
}

aether_status_t aether_service_manager_shutdown(void){
    for(size_t i=service_count;i>0;i--){
        aether_service_runtime_t *s=services[i-1];
        if(!s || s->pid<=0) continue;
        if(aether_service_stop(s)==AETHER_ERR_BUSY) continue;
    }
    for(size_t i=service_count;i>0;i--){
        aether_service_runtime_t *s=services[i-1];
        if(!s || s->pid<=0) continue;
        int status;
        pid_t r=waitpid(s->pid,&status,0);
        if(r>0) (void)aether_service_reap(r,status);
    }
    for(size_t i=0;i<service_count;i++) services[i]=NULL;
    service_count=0;
    return AETHER_OK;
}

size_t aether_service_count(void){return service_count;}
