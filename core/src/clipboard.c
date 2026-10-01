#include "aether/clipboard.h"
#include <stdlib.h>
#include <string.h>
static char *value; static aether_id_t owner;
aether_status_t aether_clipboard_init(void){value=0;owner=0;return AETHER_OK;}
aether_status_t aether_clipboard_set_text(aether_id_t o,const char*t){if(!o||!t)return AETHER_ERR_INVALID;char*n=strdup(t);if(!n)return AETHER_ERR_NOMEM;free(value);value=n;owner=o;return AETHER_OK;}
aether_status_t aether_clipboard_get_text(aether_id_t requester,const char**text){if(!text||!requester)return AETHER_ERR_INVALID;if(!value)return AETHER_ERR_NOT_FOUND;*text=value;return AETHER_OK;}
void aether_clipboard_clear(void){free(value);value=0;owner=0;}
void aether_clipboard_shutdown(void){aether_clipboard_clear();}
