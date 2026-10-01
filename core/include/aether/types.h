#ifndef AETHER_TYPES_H
#define AETHER_TYPES_H
#include <stdint.h>
typedef uint64_t aether_id_t;
typedef uint64_t aether_timestamp_t;
typedef enum {
 AETHER_OK=0, AETHER_ERR_INVALID=1, AETHER_ERR_UNAVAILABLE=2,
 AETHER_ERR_PERMISSION=3, AETHER_ERR_NOT_FOUND=4, AETHER_ERR_IO=5
} aether_status_t;
#endif
