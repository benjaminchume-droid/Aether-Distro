#include "aether/authentication_service.h"
#include <stdio.h>
#include <string.h>
#include "aether/event_bus.h"

#define MAX_AUTH_PROVIDERS 32
#define MAX_AUTH_CHALLENGES 128
#define AUTH_PROVIDER_NAME_MAX 128

static aether_auth_provider_t providers[MAX_AUTH_PROVIDERS];
static char provider_names[MAX_AUTH_PROVIDERS][AUTH_PROVIDER_NAME_MAX];
static aether_auth_challenge_t challenges[MAX_AUTH_CHALLENGES];
static size_t provider_count;
static uint64_t next_challenge=1;

static aether_auth_provider_t *find_provider(aether_auth_method_t method){
    for(size_t i=0;i<provider_count;i++)
        if(providers[i].method==method) return &providers[i];
    return NULL;
}

static aether_auth_challenge_t *find_challenge(uint64_t id){
    for(size_t i=0;i<MAX_AUTH_CHALLENGES;i++)
        if(challenges[i].challenge_id==id) return &challenges[i];
    return NULL;
}

static aether_auth_result_t status_to_result(aether_status_t status){
    if(status==AETHER_OK) return AETHER_AUTH_RESULT_SUCCESS;
    if(status==AETHER_ERR_UNAVAILABLE) return AETHER_AUTH_RESULT_UNAVAILABLE;
    return AETHER_AUTH_RESULT_FAILURE;
}

static void publish(const aether_auth_challenge_t *challenge){
    if(!challenge) return;
    aether_event_t event={AETHER_EVENT_AUTH_REQUESTED,challenge->user_id,challenge,sizeof(*challenge)};
    aether_event_publish(&event);
}

aether_status_t aether_authentication_init(void){
    provider_count=0;
    next_challenge=1;
    memset(providers,0,sizeof(providers));
    memset(provider_names,0,sizeof(provider_names));
    memset(challenges,0,sizeof(challenges));
    return AETHER_OK;
}

aether_status_t aether_authentication_register(const aether_auth_provider_t *provider){
    if(!provider || !provider->provider || !provider->provider[0] || provider->method==0)
        return AETHER_ERR_INVALID;
    if(strlen(provider->provider)>=AUTH_PROVIDER_NAME_MAX) return AETHER_ERR_LIMIT;
    if(provider_count>=MAX_AUTH_PROVIDERS) return AETHER_ERR_LIMIT;
    if(find_provider(provider->method)) return AETHER_ERR_EXISTS;

    size_t i=provider_count++;
    providers[i]=*provider;
    snprintf(provider_names[i],sizeof(provider_names[i]),"%s",provider->provider);
    providers[i].provider=provider_names[i];
    return AETHER_OK;
}

aether_status_t aether_authentication_begin(const aether_auth_request_t *request,uint64_t *challenge_id){
    if(!request || !request->user_id || !request->method || !challenge_id) return AETHER_ERR_INVALID;
    aether_auth_provider_t *provider=find_provider(request->method);
    if(!provider) return AETHER_ERR_UNAVAILABLE;

    for(size_t i=0;i<MAX_AUTH_CHALLENGES;i++) if(challenges[i].challenge_id==0){
        challenges[i].challenge_id=next_challenge++;
        if(challenges[i].challenge_id==0) challenges[i].challenge_id=next_challenge++;
        challenges[i].user_id=request->user_id;
        challenges[i].method=request->method;
        challenges[i].result=AETHER_AUTH_RESULT_PENDING;
        *challenge_id=challenges[i].challenge_id;

        if(provider->begin){
            aether_status_t st=provider->begin(request->user_id,*challenge_id,provider->context);
            if(st!=AETHER_OK) challenges[i].result=status_to_result(st);
        }
        publish(&challenges[i]);
        return AETHER_OK;
    }
    return AETHER_ERR_LIMIT;
}

aether_status_t aether_authentication_submit(uint64_t challenge_id,const void *credential,size_t credential_size){
    aether_auth_challenge_t *challenge=find_challenge(challenge_id);
    if(!challenge) return AETHER_ERR_NOT_FOUND;
    if(challenge->result!=AETHER_AUTH_RESULT_PENDING) return AETHER_ERR_STATE;

    aether_auth_provider_t *provider=find_provider(challenge->method);
    if(!provider || !provider->verify) return AETHER_ERR_UNAVAILABLE;

    aether_status_t status=provider->verify(challenge->user_id,challenge->challenge_id,
                                            credential,credential_size,provider->context);
    challenge->result=status_to_result(status);
    publish(challenge);
    return AETHER_OK;
}

aether_status_t aether_authentication_complete(uint64_t challenge_id,aether_auth_result_t result){
    if(!challenge_id || result==AETHER_AUTH_RESULT_PENDING) return AETHER_ERR_INVALID;
    aether_auth_challenge_t *challenge=find_challenge(challenge_id);
    if(!challenge) return AETHER_ERR_NOT_FOUND;
    if(challenge->result!=AETHER_AUTH_RESULT_PENDING) return AETHER_ERR_STATE;
    challenge->result=result;
    publish(challenge);
    return AETHER_OK;
}

aether_status_t aether_authentication_get(uint64_t challenge_id,aether_auth_challenge_t *out){
    if(!challenge_id || !out) return AETHER_ERR_INVALID;
    aether_auth_challenge_t *challenge=find_challenge(challenge_id);
    if(!challenge) return AETHER_ERR_NOT_FOUND;
    *out=*challenge;
    return AETHER_OK;
}

size_t aether_authentication_provider_count(void){return provider_count;}

void aether_authentication_shutdown(void){
    for(size_t i=0;i<MAX_AUTH_CHALLENGES;i++){
        if(challenges[i].challenge_id && challenges[i].result==AETHER_AUTH_RESULT_PENDING){
            aether_auth_provider_t *provider=find_provider(challenges[i].method);
            if(provider && provider->cancel)
                provider->cancel(challenges[i].user_id,challenges[i].challenge_id,provider->context);
        }
    }
    provider_count=0;
    memset(providers,0,sizeof(providers));
    memset(provider_names,0,sizeof(provider_names));
    memset(challenges,0,sizeof(challenges));
}
