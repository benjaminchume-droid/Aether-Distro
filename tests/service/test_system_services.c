#include <assert.h>
#include "aether/system_services.h"
#include "aether/event_bus.h"
static unsigned events;
static void on_event(const aether_event_t *e,void *ctx){(void)ctx;if(e->kind==AETHER_EVENT_SYSTEM_SERVICE_CHANGED)events++;}
int main(void){
 assert(aether_event_bus_init()==AETHER_OK);
 assert(aether_system_services_init()==AETHER_OK);
 aether_event_subscription_t sub={AETHER_EVENT_SYSTEM_SERVICE_CHANGED,on_event,0};
 assert(aether_event_subscribe(&sub)==AETHER_OK);
 assert(aether_system_service_set_available(AETHER_SYSTEM_NETWORK,1)==AETHER_OK);
 assert(events==1);
 aether_system_service_status_t s;
 assert(aether_system_service_get(AETHER_SYSTEM_NETWORK,&s)==AETHER_OK);
 assert(s.available==1&&s.generation==1);
 assert(aether_system_service_set_available(AETHER_SYSTEM_NETWORK,1)==AETHER_OK);
 assert(events==1);
 aether_system_services_shutdown(); aether_event_bus_shutdown(); return 0;
}