#ifndef AETHER_RUNTIME_H
#define AETHER_RUNTIME_H
#include "types.h"
typedef enum {
 AETHER_RUNTIME_NATIVE=1,
 AETHER_RUNTIME_WINDOWS,
 AETHER_RUNTIME_ANDROID
} aether_runtime_kind_t;
typedef struct {
 aether_runtime_kind_t kind;
 const char *name;
 uint32_t flags;
} aether_runtime_t;
#endif
