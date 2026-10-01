#ifndef AETHER_BATTERY_H
#define AETHER_BATTERY_H
#include <stddef.h>
#include <aether/types.h>
typedef struct { char name[64]; int capacity; int status; } aether_battery_t;
aether_status_t aether_battery_scan(void); size_t aether_battery_count(void); const aether_battery_t*aether_battery_get(size_t index);
#endif
