#include <aether/capability_registry.h>

#define AETHER_MAX_CAPABILITIES 128

static aether_capability_record_t registry[AETHER_MAX_CAPABILITIES];
static size_t registry_count;

aether_status_t aether_capability_registry_init(void) {
    registry_count = 0;
    return AETHER_OK;
}

aether_status_t aether_capability_register(const aether_capability_record_t *record) {
    if (!record || !record->name || registry_count >= AETHER_MAX_CAPABILITIES)
        return AETHER_ERR_INVALID;
    registry[registry_count++] = *record;
    return AETHER_OK;
}

size_t aether_capability_count(void) {
    return registry_count;
}

const aether_capability_record_t *aether_capability_get(size_t index) {
    if (index >= registry_count) return NULL;
    return &registry[index];
}
