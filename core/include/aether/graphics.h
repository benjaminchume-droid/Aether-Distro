#ifndef AETHER_GRAPHICS_H
#define AETHER_GRAPHICS_H
#include "types.h"
typedef struct {
 aether_id_t device_id;
 const char *vendor;
 const char *renderer;
 uint32_t api_version;
 uint32_t flags;
} aether_gpu_info_t;
#endif
