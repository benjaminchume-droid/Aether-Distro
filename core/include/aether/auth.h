#ifndef AETHER_AUTH_H
#define AETHER_AUTH_H
#include <stddef.h>
#include "types.h"

typedef enum {
 AETHER_AUTH_PASSWORD=1,
 AETHER_AUTH_PIN,
 AETHER_AUTH_FINGERPRINT,
 AETHER_AUTH_FACE,
 AETHER_AUTH_SECURITY_KEY,
 AETHER_AUTH_SMART_CARD,
 AETHER_AUTH_TPM
} aether_auth_method_t;

typedef enum {
 AETHER_AUTH_RESULT_PENDING=1,
 AETHER_AUTH_RESULT_SUCCESS,
 AETHER_AUTH_RESULT_FAILURE,
 AETHER_AUTH_RESULT_UNAVAILABLE,
 AETHER_AUTH_RESULT_CANCELLED
} aether_auth_result_t;

typedef struct {
 aether_auth_method_t method;
 uint32_t flags;
 const char *provider;
 aether_status_t (*begin)(aether_id_t user_id,uint64_t challenge_id,void *context);
 aether_status_t (*verify)(aether_id_t user_id,uint64_t challenge_id,const void *credential,size_t credential_size,void *context);
 void (*cancel)(aether_id_t user_id,uint64_t challenge_id,void *context);
 void *context;
} aether_auth_provider_t;

typedef struct {
 aether_id_t user_id;
 aether_auth_method_t method;
 uint64_t challenge_id;
} aether_auth_request_t;
#endif
