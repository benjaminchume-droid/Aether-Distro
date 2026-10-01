#include "aether/clipboard.h"
#include "aether/event_bus.h"
#include <stdlib.h>
#include <string.h>

static char *value;
static aether_id_t owner;

aether_status_t aether_clipboard_init(void){value=0;owner=0;return AETHER_OK;}

aether_status_t aether_clipboard_set_text(aether_id_t o,const char*t){
 if(!o||!t)return AETHER_ERR_INVALID;
 size_t len=strlen(t)+1;
 char*n=malloc(len);
 if(!n)return AETHER_ERR_NOMEM;
 memcpy(n,t,len);
 free(value); value=n; owner=o;
 aether_event_t e={AETHER_EVENT_CLIPBOARD_CHANGED,o,value,len};
 aether_event_publish(&e);
 return AETHER_OK;
}

aether_status_t aether_clipboard_get_text(aether_id_t requester,const char**text){
 if(!text||!requester)return AETHER_ERR_INVALID;
 if(!value)return AETHER_ERR_NOT_FOUND;
 *text=value;
 return AETHER_OK;
}

void aether_clipboard_clear(void){
 free(value);value=0;owner=0;
 aether_event_t e={AETHER_EVENT_CLIPBOARD_CHANGED,0,0,0};
 aether_event_publish(&e);
}
void aether_clipboard_shutdown(void){free(value);value=0;owner=0;}
