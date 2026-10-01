#include "aether/search.h"
#include <string.h>
#define MAX_SEARCH_ITEMS 512
static aether_search_item_t items[MAX_SEARCH_ITEMS];static size_t count;
aether_status_t aether_search_init(void){count=0;return AETHER_OK;}
aether_status_t aether_search_register(const aether_search_item_t*i){if(!i||!i->id||!i->title||!i->target)return AETHER_ERR_INVALID;if(count>=MAX_SEARCH_ITEMS)return AETHER_ERR_LIMIT;items[count++]=*i;return AETHER_OK;}
aether_status_t aether_search_query(const char*q,size_t index,aether_search_item_t*out){if(!q||!out)return AETHER_ERR_INVALID;size_t hit=0;for(size_t i=0;i<count;i++)if(strstr(items[i].title,q)||strstr(items[i].target,q)){if(hit++==index){*out=items[i];return AETHER_OK;}}return AETHER_ERR_NOT_FOUND;}
void aether_search_shutdown(void){count=0;}
