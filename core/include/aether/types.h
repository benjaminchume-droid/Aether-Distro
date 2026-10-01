#ifndef AETHER_TYPES_H
#define AETHER_TYPES_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
typedef uint64_t aether_id_t;
typedef uint64_t aether_timestamp_t;
typedef enum {
 AETHER_OK=0, AETHER_ERR_INVALID=1, AETHER_ERR_UNAVAILABLE=2,
 AETHER_ERR_PERMISSION=3, AETHER_ERR_NOT_FOUND=4, AETHER_ERR_IO=5,
 AETHER_ERR_BUSY=6, AETHER_ERR_EXISTS=7, AETHER_ERR_NOMEM=8,
 AETHER_ERR_TIMEOUT=9, AETHER_ERR_STATE=10, AETHER_ERR_LIMIT=11
} aether_status_t;
const char *aether_status_string(aether_status_t status);
#endif
