#ifndef AETHER_BIOMETRIC_SERVICE_H
#define AETHER_BIOMETRIC_SERVICE_H

#include <stddef.h>
#include <stdint.h>
#include <aether/types.h>

typedef enum {
    AETHER_BIOMETRIC_FINGERPRINT = 1,
    AETHER_BIOMETRIC_FACE
} aether_biometric_kind_t;

typedef struct {
    aether_id_t provider_id;
    aether_biometric_kind_t kind;
    char name[128];
    char provider[128];
    uint32_t capabilities;
    unsigned available;
} aether_biometric_provider_t;

aether_status_t aether_biometric_init(void);
aether_status_t aether_biometric_register(const aether_biometric_provider_t *provider);
aether_status_t aether_biometric_unregister(aether_id_t provider_id);
size_t aether_biometric_count(void);
aether_status_t aether_biometric_get(size_t index,aether_biometric_provider_t *out);
const aether_biometric_provider_t *aether_biometric_find(aether_biometric_kind_t kind);
void aether_biometric_shutdown(void);
#endif
