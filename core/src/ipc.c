#include "aether/ipc.h"

#define AETHER_IPC_MAX_ENDPOINTS 128

static aether_ipc_endpoint_t endpoints[AETHER_IPC_MAX_ENDPOINTS];
static size_t endpoint_count;

aether_status_t aether_ipc_init(void) {
    endpoint_count = 0;
    return AETHER_OK;
}

aether_status_t aether_ipc_register(const aether_ipc_endpoint_t *endpoint) {
    if (!endpoint || !endpoint->handler) return AETHER_ERR_INVALID;
    if (endpoint_count >= AETHER_IPC_MAX_ENDPOINTS) return AETHER_ERR_LIMIT;
    for (size_t i = 0; i < endpoint_count; ++i)
        if (endpoints[i].method == endpoint->method) return AETHER_ERR_EXISTS;
    endpoints[endpoint_count++] = *endpoint;
    return AETHER_OK;
}

aether_status_t aether_ipc_dispatch(const aether_ipc_request_t *request,
                                    aether_ipc_response_t *response) {
    if (!request || !response) return AETHER_ERR_INVALID;
    for (size_t i = 0; i < endpoint_count; ++i) {
        if (endpoints[i].method == request->method)
            return endpoints[i].handler(request, response, endpoints[i].context);
    }
    return AETHER_ERR_NOT_FOUND;
}

void aether_ipc_shutdown(void) {
    endpoint_count = 0;
}
