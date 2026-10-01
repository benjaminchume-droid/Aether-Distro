#ifndef AETHER_SERVICE_H
#define AETHER_SERVICE_H
#include "types.h"
typedef enum { AETHER_SERVICE_STOPPED=0, AETHER_SERVICE_STARTING, AETHER_SERVICE_RUNNING, AETHER_SERVICE_FAILED } aether_service_state_t;
typedef aether_status_t (*aether_service_start_fn)(void *);
typedef aether_status_t (*aether_service_stop_fn)(void *);
typedef aether_status_t (*aether_service_health_fn)(void *);
typedef struct { const char *name; aether_service_state_t state; uint32_t restart_policy; aether_service_start_fn start; aether_service_stop_fn stop; aether_service_health_fn health; void *context; } aether_service_t;
#endif
