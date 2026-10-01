#ifndef AETHER_AUDIO_SERVICE_H
#define AETHER_AUDIO_SERVICE_H
#include <stddef.h>
#include "types.h"
#include "audio.h"
aether_status_t aether_audio_service_refresh(void);
aether_status_t aether_audio_service_count(size_t *count);
aether_status_t aether_audio_service_get(size_t index,aether_audio_device_t *out);
#endif