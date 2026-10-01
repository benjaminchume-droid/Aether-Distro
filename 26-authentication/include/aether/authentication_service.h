#ifndef AETHER_AUTHENTICATION_SERVICE_H
#define AETHER_AUTHENTICATION_SERVICE_H

#include <stddef.h>
#include <stdint.h>
#include <aether/types.h>
#include <aether/auth.h>

typedef struct {
    uint64_t challenge_id;
    aether_id_t user_id;
    aether_auth_method_t method;
    aether_auth_result_t result;
} aether_auth_challenge_t;

aether_status_t aether_authentication_init(void);
aether_status_t aether_authentication_register(const aether_auth_provider_t *provider);
aether_status_t aether_authentication_begin(const aether_auth_request_t *request,uint64_t *challenge_id);
aether_status_t aether_authentication_submit(uint64_t challenge_id,const void *credential,size_t credential_size);
aether_status_t aether_authentication_complete(uint64_t challenge_id,aether_auth_result_t result);
aether_status_t aether_authentication_get(uint64_t challenge_id,aether_auth_challenge_t *out);
size_t aether_authentication_provider_count(void);
void aether_authentication_shutdown(void);
#endif
