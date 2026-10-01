#ifndef AETHER_SERVICE_MANAGER_H
#define AETHER_SERVICE_MANAGER_H
#include <stddef.h>
#include <sys/types.h>
#include <aether/types.h>

typedef enum {
    AETHER_SERVICE_STARTING = 0,
    AETHER_SERVICE_RUNNING,
    AETHER_SERVICE_FAILED,
    AETHER_SERVICE_STOPPED
} aether_service_state_t;

typedef struct {
    aether_id_t id;
    const char *name;
    const char *exec_path;
    char *const *argv;
    const char *const *dependencies;
    size_t dependency_count;
    aether_service_state_t state;
    pid_t pid;
    int restart_on_failure;
} aether_service_runtime_t;

aether_status_t aether_service_manager_init(void);
aether_status_t aether_service_register(aether_service_runtime_t *service);
aether_status_t aether_service_start(aether_service_runtime_t *service);
aether_status_t aether_service_stop(aether_service_runtime_t *service);
aether_status_t aether_service_reap(pid_t pid, int status);
aether_status_t aether_service_manager_shutdown(void);
size_t aether_service_count(void);
#endif
