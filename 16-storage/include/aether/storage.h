#ifndef AETHER_STORAGE_H
#define AETHER_STORAGE_H
#include <stddef.h>
#include <stdint.h>
#include <aether/types.h>
typedef struct { char name[64]; char devnode[128]; uint64_t size_bytes; unsigned removable; } aether_block_device_t;
aether_status_t aether_storage_scan(void); size_t aether_storage_count(void); const aether_block_device_t*aether_storage_get(size_t index);
#endif
