#ifndef AETHER_SERVICE_MANAGER_H
#define AETHER_SERVICE_MANAGER_H

#include <stddef.h>
#include <aether/types.h>
#include <aether/service.h>

typedef enum {
    AETHER_SERVICE_STARTING = 0,
    AETHER_SERVICE_RUNNING,
    AETHER_SERVICE_FAILED,
    AETHER_SERVICE_STOPPED
} aether_service_state_t;

typedef struct {
    aether_service_id_t id;
    const char *name;
    aether_service_state_t state;
    const char *const *dependencies;
    size_t dependency_count;
} aether_service_runtime_t;

aether_status_t aether_service_manager_init(void);
aether_status_t aether_service_start(aether_service_runtime_t *service);
aether_status_t aether_service_stop(aether_service_runtime_t *service);
aether_status_t aether_service_manager_shutdown(void);

#endif
