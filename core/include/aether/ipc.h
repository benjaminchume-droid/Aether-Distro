#ifndef AETHER_IPC_H
#define AETHER_IPC_H
#include "types.h"
typedef uint32_t aether_ipc_method_t;
typedef struct { aether_ipc_method_t method; const void *payload; uint64_t payload_size; } aether_ipc_request_t;
typedef struct { aether_status_t status; const void *payload; uint64_t payload_size; } aether_ipc_response_t;
#endif
