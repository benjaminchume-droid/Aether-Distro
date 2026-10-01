#ifndef AETHER_PERMISSION_H
#define AETHER_PERMISSION_H
#include "types.h"
typedef enum {
 AETHER_PERMISSION_FILES=1,
 AETHER_PERMISSION_NETWORK,
 AETHER_PERMISSION_CAMERA,
 AETHER_PERMISSION_MICROPHONE,
 AETHER_PERMISSION_BLUETOOTH,
 AETHER_PERMISSION_USB,
 AETHER_PERMISSION_LOCATION
} aether_permission_kind_t;
typedef struct {
 aether_id_t app_id;
 aether_permission_kind_t permission;
 uint32_t state;
} aether_permission_t;
#endif
