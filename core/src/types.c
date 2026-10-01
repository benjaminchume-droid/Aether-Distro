#include "aether/types.h"

const char *aether_status_string(aether_status_t status) {
    switch (status) {
    case AETHER_OK: return "ok";
    case AETHER_ERR_INVALID: return "invalid";
    case AETHER_ERR_UNAVAILABLE: return "unavailable";
    case AETHER_ERR_PERMISSION: return "permission denied";
    case AETHER_ERR_NOT_FOUND: return "not found";
    case AETHER_ERR_IO: return "io error";
    case AETHER_ERR_BUSY: return "busy";
    case AETHER_ERR_EXISTS: return "already exists";
    case AETHER_ERR_NOMEM: return "out of memory";
    case AETHER_ERR_TIMEOUT: return "timeout";
    case AETHER_ERR_STATE: return "invalid state";
    case AETHER_ERR_LIMIT: return "limit exceeded";
    default: return "unknown";
    }
}
