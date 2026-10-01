#include "aether/system_services.h"

static aether_system_service_status_t services[] = {
    {AETHER_SYSTEM_STORAGE, "storage", 0},
    {AETHER_SYSTEM_NETWORK, "network", 0},
    {AETHER_SYSTEM_AUDIO, "audio", 0},
    {AETHER_SYSTEM_VIDEO, "video", 0},
    {AETHER_SYSTEM_SESSION, "session", 0},
    {AETHER_SYSTEM_NOTIFICATION, "notification", 0},
    {AETHER_SYSTEM_CLIPBOARD, "clipboard", 0},
    {AETHER_SYSTEM_SEARCH, "search", 0}
};

aether_status_t aether_system_services_init(void) {
    for (size_t i = 0; i < sizeof(services)/sizeof(services[0]); ++i)
        services[i].available = 0;
    return AETHER_OK;
}

aether_status_t aether_system_service_set_available(aether_system_service_kind_t kind, uint32_t available) {
    for (size_t i = 0; i < sizeof(services)/sizeof(services[0]); ++i)
        if (services[i].kind == kind) { services[i].available = available ? 1u : 0u; return AETHER_OK; }
    return AETHER_ERR_NOT_FOUND;
}

aether_status_t aether_system_service_get(aether_system_service_kind_t kind, aether_system_service_status_t *out) {
    if (!out) return AETHER_ERR_INVALID;
    for (size_t i = 0; i < sizeof(services)/sizeof(services[0]); ++i)
        if (services[i].kind == kind) { *out = services[i]; return AETHER_OK; }
    return AETHER_ERR_NOT_FOUND;
}

void aether_system_services_shutdown(void) {
    for (size_t i = 0; i < sizeof(services)/sizeof(services[0]); ++i) services[i].available = 0;
}
