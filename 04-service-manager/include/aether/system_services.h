#ifndef AETHER_SYSTEM_SERVICES_H
#define AETHER_SYSTEM_SERVICES_H
#include "types.h"
typedef enum {
 AETHER_SYSTEM_STORAGE=1,AETHER_SYSTEM_NETWORK,AETHER_SYSTEM_AUDIO,AETHER_SYSTEM_VIDEO,
 AETHER_SYSTEM_SESSION,AETHER_SYSTEM_NOTIFICATION,AETHER_SYSTEM_CLIPBOARD,AETHER_SYSTEM_SEARCH
} aether_system_service_kind_t;
typedef struct { aether_system_service_kind_t kind; const char *name; uint32_t available; uint32_t generation; } aether_system_service_status_t;
aether_status_t aether_system_services_init(void);
aether_status_t aether_system_service_set_available(aether_system_service_kind_t kind,uint32_t available);
aether_status_t aether_system_service_get(aether_system_service_kind_t kind,aether_system_service_status_t *out);
aether_status_t aether_system_services_refresh(void);
void aether_system_services_shutdown(void);
#endif