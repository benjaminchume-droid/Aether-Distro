#include "aether/system_services.h"
#include "aether/storage.h"
#include "aether/network.h"
#include "aether/audio.h"
#include "aether/camera.h"
#include "aether/event_bus.h"
#include <string.h>

static aether_system_service_status_t services[]={
 {AETHER_SYSTEM_STORAGE,"storage",0,0},{AETHER_SYSTEM_NETWORK,"network",0,0},
 {AETHER_SYSTEM_AUDIO,"audio",0,0},{AETHER_SYSTEM_VIDEO,"video",0,0},
 {AETHER_SYSTEM_SESSION,"session",0,0},{AETHER_SYSTEM_NOTIFICATION,"notification",0,0},
 {AETHER_SYSTEM_CLIPBOARD,"clipboard",0,0},{AETHER_SYSTEM_SEARCH,"search",0,0}
};
static aether_system_service_status_t *find(aether_system_service_kind_t k){
 for(size_t i=0;i<sizeof(services)/sizeof(services[0]);i++) if(services[i].kind==k)return &services[i]; return NULL;
}
aether_status_t aether_system_services_init(void){for(size_t i=0;i<sizeof(services)/sizeof(services[0]);i++){services[i].available=0;services[i].generation=0;}return AETHER_OK;}
aether_status_t aether_system_service_set_available(aether_system_service_kind_t k,uint32_t available){
 aether_system_service_status_t *s=find(k); if(!s)return AETHER_ERR_NOT_FOUND;
 if(s->available!=(available?1u:0u)){s->available=available?1u:0u;s->generation++;
  aether_event_t e={AETHER_EVENT_SYSTEM_SERVICE_CHANGED,0,s,sizeof(*s)}; aether_event_publish(&e);}
 return AETHER_OK;
}
aether_status_t aether_system_service_get(aether_system_service_kind_t k,aether_system_service_status_t *out){if(!out)return AETHER_ERR_INVALID;aether_system_service_status_t*s=find(k);if(!s)return AETHER_ERR_NOT_FOUND;*out=*s;return AETHER_OK;}
aether_status_t aether_system_services_refresh(void){
 aether_system_service_set_available(AETHER_SYSTEM_STORAGE,aether_storage_scan()==AETHER_OK&&aether_storage_count()>0);
 aether_system_service_set_available(AETHER_SYSTEM_NETWORK,aether_network_scan()==AETHER_OK&&aether_network_count()>0);
 aether_system_service_set_available(AETHER_SYSTEM_AUDIO,aether_audio_scan()==AETHER_OK&&aether_audio_count()>0);
 aether_system_service_set_available(AETHER_SYSTEM_VIDEO,aether_camera_scan()==AETHER_OK&&aether_camera_count()>0);
 return AETHER_OK;
}
void aether_system_services_shutdown(void){for(size_t i=0;i<sizeof(services)/sizeof(services[0]);i++){services[i].available=0;services[i].generation++;}}
