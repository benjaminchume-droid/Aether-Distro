#ifndef AETHER_CLIPBOARD_H
#define AETHER_CLIPBOARD_H
#include <stddef.h>
#include "types.h"
aether_status_t aether_clipboard_init(void);
aether_status_t aether_clipboard_set_text(aether_id_t owner,const char *text);
aether_status_t aether_clipboard_get_text(aether_id_t requester,const char **text);
void aether_clipboard_clear(void);
void aether_clipboard_shutdown(void);
#endif