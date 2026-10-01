#ifndef AETHER_CAPABILITY_H
#define AETHER_CAPABILITY_H
#include "types.h"
typedef enum {
 AETHER_CAP_CPU=1, AETHER_CAP_GPU, AETHER_CAP_DISPLAY, AETHER_CAP_AUDIO,
 AETHER_CAP_CAMERA, AETHER_CAP_FINGERPRINT, AETHER_CAP_FACE_AUTH,
 AETHER_CAP_TPM, AETHER_CAP_SECURITY_KEY, AETHER_CAP_BATTERY,
 AETHER_CAP_WIFI, AETHER_CAP_BLUETOOTH, AETHER_CAP_TOUCH,
 AETHER_CAP_EXTERNAL_DISPLAY
} aether_capability_kind_t;
typedef struct {
 aether_capability_kind_t kind; aether_id_t device_id; uint32_t flags; const char *name;
} aether_capability_t;
#endif
