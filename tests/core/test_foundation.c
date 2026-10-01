#include <assert.h>
#include <string.h>
#include "aether/types.h"
#include "aether/ipc.h"
#include "aether/event_bus.h"
#include "aether/capability_registry.h"

static int event_seen;
static aether_status_t handler(const aether_ipc_request_t *r,aether_ipc_response_t *s,void *ctx){
 (void)r; (void)ctx; s->status=AETHER_OK; s->payload=0; s->payload_size=0; return AETHER_OK;
}
static void on_event(const aether_event_t *e,void *ctx){(void)ctx;if(e->kind==AETHER_EVENT_DEVICE_ADDED)event_seen++;}

int main(void){
 assert(strcmp(aether_status_string(AETHER_OK),"ok")==0);
 assert(strcmp(aether_status_string(AETHER_ERR_LIMIT),"limit exceeded")==0);

 assert(aether_ipc_init()==AETHER_OK);
 aether_ipc_endpoint_t ep={42,handler,0}; assert(aether_ipc_register(&ep)==AETHER_OK);
 aether_ipc_response_t response={0}; aether_ipc_request_t request={1,2,42,0,0};
 assert(aether_ipc_dispatch(&request,&response)==AETHER_OK);
 assert(response.status==AETHER_OK);
 assert(aether_ipc_dispatch(&(aether_ipc_request_t){1,2,99,0,0},&response)==AETHER_ERR_NOT_FOUND);
 aether_ipc_shutdown();

 assert(aether_event_bus_init()==AETHER_OK);
 aether_event_subscription_t sub={AETHER_EVENT_DEVICE_ADDED,on_event,0};
 assert(aether_event_subscribe(&sub)==AETHER_OK);
 aether_event_t event={AETHER_EVENT_DEVICE_ADDED,7,0,0};
 assert(aether_event_publish(&event)==AETHER_OK && event_seen==1);
 aether_event_bus_shutdown();

 assert(aether_capability_registry_init()==AETHER_OK);
 aether_capability_record_t cap={AETHER_CAP_CPU,10,0,"cpu0","kernel",1};
 assert(aether_capability_register(&cap)==AETHER_OK);
 assert(aether_capability_count()==1);
 assert(aether_capability_find(10,AETHER_CAP_CPU)!=0);
 assert(aether_capability_register(&cap)==AETHER_ERR_EXISTS);
 assert(aether_capability_unregister(10,AETHER_CAP_CPU)==AETHER_OK);
 assert(aether_capability_count()==0);
 return 0;
}
