#include "../include/aether/biometric_service.h"
#include <string.h>

#define MAX_BIOMETRIC_PROVIDERS 32

static aether_biometric_provider_t providers[MAX_BIOMETRIC_PROVIDERS];
static size_t count;

aether_status_t aether_biometric_init(void){
    count=0;
    memset(providers,0,sizeof(providers));
    return AETHER_OK;
}

aether_status_t aether_biometric_register(const aether_biometric_provider_t *provider){
    if(!provider || !provider->provider_id || !provider->kind || !provider->provider || !provider->name)
        return AETHER_ERR_INVALID;
    if(provider->kind!=AETHER_BIOMETRIC_FINGERPRINT && provider->kind!=AETHER_BIOMETRIC_FACE)
        return AETHER_ERR_INVALID;
    if(count>=MAX_BIOMETRIC_PROVIDERS) return AETHER_ERR_LIMIT;
    for(size_t i=0;i<count;i++) if(providers[i].provider_id==provider->provider_id)
        return AETHER_ERR_EXISTS;
    providers[count++]=*provider;
    return AETHER_OK;
}

aether_status_t aether_biometric_unregister(aether_id_t provider_id){
    if(!provider_id) return AETHER_ERR_INVALID;
    for(size_t i=0;i<count;i++) if(providers[i].provider_id==provider_id){
        providers[i]=providers[--count];
        memset(&providers[count],0,sizeof(providers[count]));
        return AETHER_OK;
    }
    return AETHER_ERR_NOT_FOUND;
}

size_t aether_biometric_count(void){return count;}

aether_status_t aether_biometric_get(size_t index,aether_biometric_provider_t *out){
    if(!out) return AETHER_ERR_INVALID;
    if(index>=count) return AETHER_ERR_NOT_FOUND;
    *out=providers[index];
    return AETHER_OK;
}

const aether_biometric_provider_t *aether_biometric_find(aether_biometric_kind_t kind){
    for(size_t i=0;i<count;i++) if(providers[i].kind==kind && providers[i].available) return &providers[i];
    return NULL;
}

void aether_biometric_shutdown(void){count=0;memset(providers,0,sizeof(providers));}
