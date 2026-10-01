#ifndef AETHER_DISPLAY_H
#define AETHER_DISPLAY_H
#include <stddef.h>
#include <aether/types.h>
typedef struct { char connector[64]; unsigned connected,width,height,refresh_millihz; } aether_display_t;
aether_status_t aether_display_scan(void); size_t aether_display_count(void); const aether_display_t*aether_display_get(size_t index);
#endif
