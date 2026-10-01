#ifndef AETHER_AUDIO_H
#define AETHER_AUDIO_H
#include <stddef.h>
#include <aether/types.h>
typedef struct { char id[128]; char name[128]; char type[32]; } aether_audio_device_t;
aether_status_t aether_audio_scan(void); size_t aether_audio_count(void); const aether_audio_device_t*aether_audio_get(size_t index);
#endif
