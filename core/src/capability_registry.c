#include "aether/capability_registry.h"
#include <stdio.h>
#include <string.h>

#define AETHER_MAX_CAPABILITIES 256
#define AETHER_CAPABILITY_NAME_MAX 128
#define AETHER_CAPABILITY_PROVIDER_MAX 128

static aether_capability_record_t registry[AETHER_MAX_CAPABILITIES];
static char names[AETHER_MAX_CAPABILITIES][AETHER_CAPABILITY_NAME_MAX];
static char providers[AETHER_MAX_CAPABILITIES][AETHER_CAPABILITY_PROVIDER_MAX];
static size_t count;

aether_status_t aether_capability_registry_init(void){
    count=0;
    memset(registry,0,sizeof(registry));
    memset(names,0,sizeof(names));
    memset(providers,0,sizeof(providers));
    return AETHER_OK;
}

aether_status_t aether_capability_register(const aether_capability_record_t *r){
    if(!r || !r->name || !r->name[0] || !r->provider || !r->provider[0]) return AETHER_ERR_INVALID;
    if(count>=AETHER_MAX_CAPABILITIES) return AETHER_ERR_LIMIT;
    if(aether_capability_find(r->device_id,r->kind)) return AETHER_ERR_EXISTS;

    size_t i=count++;
    registry[i]=*r;
    snprintf(names[i],sizeof(names[i]),"%s",r->name);
    snprintf(providers[i],sizeof(providers[i]),"%s",r->provider);
    registry[i].name=names[i];
    registry[i].provider=providers[i];
    return AETHER_OK;
}

aether_status_t aether_capability_unregister(aether_id_t id,aether_capability_kind_t kind){
    for(size_t i=0;i<count;i++){
        if(registry[i].device_id==id && registry[i].kind==kind){
            size_t last=--count;
            if(i!=last){
                registry[i]=registry[last];
                memcpy(names[i],names[last],sizeof(names[i]));
                memcpy(providers[i],providers[last],sizeof(providers[i]));
                registry[i].name=names[i];
                registry[i].provider=providers[i];
            }
            memset(&registry[last],0,sizeof(registry[last]));
            memset(names[last],0,sizeof(names[last]));
            memset(providers[last],0,sizeof(providers[last]));
            return AETHER_OK;
        }
    }
    return AETHER_ERR_NOT_FOUND;
}

size_t aether_capability_count(void){return count;}

const aether_capability_record_t *aether_capability_get(size_t i){
    return i<count ? &registry[i] : NULL;
}

const aether_capability_record_t *aether_capability_find(aether_id_t id,aether_capability_kind_t kind){
    for(size_t i=0;i<count;i++)
        if(registry[i].device_id==id && registry[i].kind==kind) return &registry[i];
    return NULL;
}
