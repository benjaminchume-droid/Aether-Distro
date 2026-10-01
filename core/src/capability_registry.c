#include "aether/capability_registry.h"
#define AETHER_MAX_CAPABILITIES 256
static aether_capability_record_t registry[AETHER_MAX_CAPABILITIES];
static size_t count;
aether_status_t aether_capability_registry_init(void){count=0;return AETHER_OK;}
aether_status_t aether_capability_register(const aether_capability_record_t *r){
 if(!r||!r->name||!r->provider)return AETHER_ERR_INVALID;
 if(count>=AETHER_MAX_CAPABILITIES)return AETHER_ERR_LIMIT;
 if(aether_capability_find(r->device_id,r->kind))return AETHER_ERR_EXISTS;
 registry[count++]=*r; return AETHER_OK;
}
aether_status_t aether_capability_unregister(aether_id_t id,aether_capability_kind_t kind){
 for(size_t i=0;i<count;i++) if(registry[i].device_id==id&&registry[i].kind==kind){registry[i]=registry[--count];return AETHER_OK;}
 return AETHER_ERR_NOT_FOUND;
}
size_t aether_capability_count(void){return count;}
const aether_capability_record_t *aether_capability_get(size_t i){return i<count?&registry[i]:0;}
const aether_capability_record_t *aether_capability_find(aether_id_t id,aether_capability_kind_t kind){
 for(size_t i=0;i<count;i++) if(registry[i].device_id==id&&registry[i].kind==kind)return &registry[i];
 return 0;
}
