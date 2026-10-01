#ifndef AETHER_AUTH_H
#define AETHER_AUTH_H
#include "types.h"
typedef enum {
 AETHER_AUTH_PASSWORD=1, AETHER_AUTH_PIN, AETHER_AUTH_FINGERPRINT,
 AETHER_AUTH_FACE, AETHER_AUTH_SECURITY_KEY, AETHER_AUTH_SMART_CARD,
 AETHER_AUTH_TPM
} aether_auth_method_t;
typedef struct { aether_auth_method_t method; uint32_t flags; const char *provider; } aether_auth_provider_t;
typedef struct { aether_id_t user_id; aether_auth_method_t method; uint64_t challenge_id; } aether_auth_request_t;
#endif
