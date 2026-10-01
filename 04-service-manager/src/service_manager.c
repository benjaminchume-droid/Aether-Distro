#include <aether/service_manager.h>

aether_status_t aether_service_manager_init(void) {
    return AETHER_OK;
}

aether_status_t aether_service_start(aether_service_runtime_t *service) {
    if (!service || !service->name) return AETHER_ERR_INVALID;
    service->state = AETHER_SERVICE_STARTING;
    service->state = AETHER_SERVICE_RUNNING;
    return AETHER_OK;
}

aether_status_t aether_service_stop(aether_service_runtime_t *service) {
    if (!service || !service->name) return AETHER_ERR_INVALID;
    service->state = AETHER_SERVICE_STOPPED;
    return AETHER_OK;
}

aether_status_t aether_service_manager_shutdown(void) {
    return AETHER_OK;
}
