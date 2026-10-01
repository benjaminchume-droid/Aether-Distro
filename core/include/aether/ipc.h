#ifndef AETHER_IPC_H
#define AETHER_IPC_H
#include <stddef.h>
#include <stdint.h>
#include "types.h"
typedef uint32_t aether_ipc_method_t;
typedef struct { aether_id_t sender; aether_id_t target; aether_ipc_method_t method; const void *payload; size_t payload_size; } aether_ipc_request_t;
typedef struct { aether_status_t status; const void *payload; size_t payload_size; } aether_ipc_response_t;
typedef aether_status_t (*aether_ipc_handler_t)(const aether_ipc_request_t *, aether_ipc_response_t *, void *);
typedef struct { aether_ipc_method_t method; aether_ipc_handler_t handler; void *context; } aether_ipc_endpoint_t;
aether_status_t aether_ipc_init(void);
aether_status_t aether_ipc_register(const aether_ipc_endpoint_t *);
aether_status_t aether_ipc_dispatch(const aether_ipc_request_t *, aether_ipc_response_t *);
void aether_ipc_shutdown(void);
#endif
