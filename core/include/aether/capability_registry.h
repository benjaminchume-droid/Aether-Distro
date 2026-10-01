#ifndef AETHER_CAPABILITY_REGISTRY_H
#define AETHER_CAPABILITY_REGISTRY_H

#include <stddef.h>
#include "capability.h"

typedef struct {
    aether_capability_kind_t kind;
    aether_id_t device_id;
    uint32_t flags;
    const char *name;
    const char *provider;
} aether_capability_record_t;

aether_status_t aether_capability_registry_init(void);
aether_status_t aether_capability_register(const aether_capability_record_t *record);
aether_status_t aether_capability_unregister(aether_id_t device_id,
                                             aether_capability_kind_t kind);
size_t aether_capability_count(void);
const aether_capability_record_t *aether_capability_get(size_t index);
const aether_capability_record_t *aether_capability_find(aether_id_t device_id,
                                                          aether_capability_kind_t kind);

#endif
