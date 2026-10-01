#include "aether/notification.h"
#include "aether/event_bus.h"
#include <string.h>
#define MAX_NOTIFICATIONS 256
#define TEXT_MAX 256

static aether_notification_t items[MAX_NOTIFICATIONS];
static char titles[MAX_NOTIFICATIONS][TEXT_MAX];
static char bodies[MAX_NOTIFICATIONS][TEXT_MAX];
static size_t count;
static aether_id_t next_id=1;

aether_status_t aether_notification_init(void){count=0;next_id=1;return AETHER_OK;}

aether_status_t aether_notification_post(aether_id_t app,const char*t,const char*b,aether_id_t*id){
 if(!app||!t||!b)return AETHER_ERR_INVALID;
 if(count>=MAX_NOTIFICATIONS)return AETHER_ERR_LIMIT;
 aether_notification_t*n=&items[count];
 n->id=next_id++; n->app_id=app;
 strncpy(titles[count],t,TEXT_MAX-1); titles[count][TEXT_MAX-1]='\0';
 strncpy(bodies[count],b,TEXT_MAX-1); bodies[count][TEXT_MAX-1]='\0';
 n->title=titles[count]; n->body=bodies[count];
 count++;
 if(id)*id=n->id;
 aether_event_t e={AETHER_EVENT_NOTIFICATION_POSTED,app,n,sizeof(*n)};
 aether_event_publish(&e);
 return AETHER_OK;
}

aether_status_t aether_notification_get(size_t i,aether_notification_t*out){
 if(!out)return AETHER_ERR_INVALID;
 if(i>=count)return AETHER_ERR_NOT_FOUND;
 *out=items[i];
 return AETHER_OK;
}
size_t aether_notification_count(void){return count;}
void aether_notification_shutdown(void){count=0;}
