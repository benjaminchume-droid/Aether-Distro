#include "aether/authentication_service.h"
#include <string.h>

#define MAX_AUTH_PROVIDERS 32
#define MAX_AUTH_CHALLENGES 128

static aether_auth_provider_t providers[MAX_AUTH_PROVIDERS];
static aether_auth_challenge_t challenges[MAX_AUTH_CHALLENGES];
static size_t provider_count;
static uint64_t next_challenge=1;

aether_status_t aether_authentication_init(void){ provider_count=0; next_challenge=1; memset(challenges,0,sizeof(challenges)); return AETHER_OK; }

aether_status_t aether_authentication_register(const aether_auth_provider_t *provider){
    if(!provider || provider->method==0 || !provider->provider) return AETHER_ERR_INVALID;
    if(provider_count>=MAX_AUTH_PROVIDERS) return AETHER_ERR_LIMIT;
    for(size_t i=0;i<provider_count;i++) if(providers[i].method==provider->method) return AETHER_ERR_EXISTS;
    providers[provider_count++]=*provider;
    return AETHER_OK;
}

static int provider_available(aether_auth_method_t method){
    for(size_t i=0;i<provider_count;i++) if(providers[i].method==method) return 1;
    return 0;
}

aether_status_t aether_authentication_begin(const aether_auth_request_t *request,uint64_t *challenge_id){
    if(!request || !request->user_id || !request->method || !challenge_id) return AETHER_ERR_INVALID;
    if(!provider_available(request->method)) return AETHER_ERR_UNAVAILABLE;
    for(size_t i=0;i<MAX_AUTH_CHALLENGES;i++) if(challenges[i].challenge_id==0){
        challenges[i].challenge_id=next_challenge++;
        if(challenges[i].challenge_id==0) challenges[i].challenge_id=next_challenge++;
        challenges[i].user_id=request->user_id;
        challenges[i].method=request->method;
        challenges[i].result=AETHER_AUTH_RESULT_PENDING;
        *challenge_id=challenges[i].challenge_id;
        return AETHER_OK;
    }
    return AETHER_ERR_LIMIT;
}

aether_status_t aether_authentication_complete(uint64_t challenge_id,aether_auth_result_t result){
    if(!challenge_id || result==AETHER_AUTH_RESULT_PENDING) return AETHER_ERR_INVALID;
    for(size_t i=0;i<MAX_AUTH_CHALLENGES;i++) if(challenges[i].challenge_id==challenge_id){
        challenges[i].result=result;
        return AETHER_OK;
    }
    return AETHER_ERR_NOT_FOUND;
}

aether_status_t aether_authentication_get(uint64_t challenge_id,aether_auth_challenge_t *out){
    if(!challenge_id || !out) return AETHER_ERR_INVALID;
    for(size_t i=0;i<MAX_AUTH_CHALLENGES;i++) if(challenges[i].challenge_id==challenge_id){
        *out=challenges[i];
        return AETHER_OK;
    }
    return AETHER_ERR_NOT_FOUND;
}

size_t aether_authentication_provider_count(void){ return provider_count; }
void aether_authentication_shutdown(void){ provider_count=0; memset(challenges,0,sizeof(challenges)); }
