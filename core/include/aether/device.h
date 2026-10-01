#ifndef AETHER_DEVICE_H
#define AETHER_DEVICE_H
#include "types.h"
#include "capability.h"
typedef struct {
 aether_id_t id; const char *name; const char *vendor; const char *class_name;
 uint32_t capability_count; const aether_capability_t *capabilities;
} aether_device_t;
#endif
