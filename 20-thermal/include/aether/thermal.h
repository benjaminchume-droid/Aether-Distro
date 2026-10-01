#ifndef AETHER_THERMAL_H
#define AETHER_THERMAL_H
#include <stddef.h>
#include <aether/types.h>
typedef struct { char name[64]; int temperature_millidegrees; } aether_thermal_zone_t;
aether_status_t aether_thermal_scan(void); size_t aether_thermal_count(void); const aether_thermal_zone_t*aether_thermal_get(size_t index);
#endif
