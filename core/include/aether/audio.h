#ifndef AETHER_AUDIO_H
#define AETHER_AUDIO_H
#include "types.h"
typedef enum { AETHER_AUDIO_PLAYBACK=1, AETHER_AUDIO_CAPTURE } aether_audio_direction_t;
typedef struct {
 aether_id_t device_id;
 aether_audio_direction_t direction;
 const char *name;
 uint32_t flags;
} aether_audio_device_t;
#endif
