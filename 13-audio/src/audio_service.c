#include "aether/audio_service.h"
aether_status_t aether_audio_service_refresh(void){return aether_audio_scan();}
aether_status_t aether_audio_service_count(size_t*out){if(!out)return AETHER_ERR_INVALID;*out=aether_audio_count();return AETHER_OK;}
aether_status_t aether_audio_service_get(size_t i,aether_audio_device_t*out){if(!out)return AETHER_ERR_INVALID;const aether_audio_device_t*d=aether_audio_get(i);if(!d)return AETHER_ERR_NOT_FOUND;*out=*d;return AETHER_OK;}
