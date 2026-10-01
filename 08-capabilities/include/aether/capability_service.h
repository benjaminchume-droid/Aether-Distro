#ifndef AETHER_CAPABILITY_SERVICE_H
#define AETHER_CAPABILITY_SERVICE_H
#include <stddef.h>
#include <aether/types.h>
#include <aether/capability.h>
#include <aether/capability_registry.h>
aether_status_t aether_capability_service_init(void);
aether_status_t aether_capability_refresh(void);
const aether_capability_record_t *aether_capability_find_kind(aether_capability_kind_t kind);
#endif
