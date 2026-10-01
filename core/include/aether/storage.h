#ifndef AETHER_STORAGE_H
#define AETHER_STORAGE_H
#include "types.h"
typedef enum { AETHER_STORAGE_DISK=1, AETHER_STORAGE_PARTITION, AETHER_STORAGE_VOLUME } aether_storage_kind_t;
typedef struct {
 aether_id_t id;
 aether_storage_kind_t kind;
 uint64_t size_bytes;
 uint32_t flags;
 const char *device_path;
} aether_storage_device_t;
#endif
