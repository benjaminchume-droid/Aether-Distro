#include <aether/capability_service.h>
#include <aether/capability_registry.h>
#include <aether/device_manager.h>

aether_status_t aether_capability_service_init(void) {
    return aether_capability_registry_init();
}

aether_status_t aether_capability_refresh(void) {
    (void)aether_device_scan();
    return AETHER_OK;
}

const aether_capability_record_t *aether_capability_find_kind(aether_capability_kind_t kind) {
    for (size_t i = 0; i < aether_capability_count(); ++i) {
        const aether_capability_record_t *r = aether_capability_get(i);
        if (r && r->kind == kind && r->available) return r;
    }
    return NULL;
}
