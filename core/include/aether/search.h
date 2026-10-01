#ifndef AETHER_SEARCH_H
#define AETHER_SEARCH_H
#include <stddef.h>
#include "types.h"
typedef enum { AETHER_SEARCH_APP=1,AETHER_SEARCH_FILE,AETHER_SEARCH_SETTING,AETHER_SEARCH_COMMAND } aether_search_kind_t;
typedef struct { aether_id_t id; aether_search_kind_t kind; const char *title; const char *target; } aether_search_item_t;
aether_status_t aether_search_init(void);
aether_status_t aether_search_register(const aether_search_item_t *item);
aether_status_t aether_search_query(const char *query,size_t index,aether_search_item_t *out);
void aether_search_shutdown(void);
#endif