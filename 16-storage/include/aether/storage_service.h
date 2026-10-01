#ifndef AETHER_STORAGE_SERVICE_H
#define AETHER_STORAGE_SERVICE_H
#include <stdint.h>
#include <stddef.h>
#include <aether/types.h>
typedef struct { char name[64]; char mountpoint[256]; uint64_t total_bytes; uint64_t available_bytes; unsigned readonly; } aether_volume_t;
aether_status_t aether_storage_service_refresh(void);
aether_status_t aether_storage_volume_count(size_t *count);
aether_status_t aether_storage_volume_get(size_t index,aether_volume_t *out);
#endif