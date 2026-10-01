#ifndef AETHER_POWER_H
#define AETHER_POWER_H
#include "types.h"
typedef enum {
 AETHER_POWER_BALANCED=1, AETHER_POWER_PERFORMANCE,
 AETHER_POWER_EFFICIENCY, AETHER_POWER_BATTERY_SAVER
} aether_power_profile_t;
typedef struct {
 uint32_t cpu_utilization, gpu_utilization, thermal_state, battery_percent;
 uint32_t display_activity, foreground_latency;
} aether_power_observation_t;
aether_status_t aether_power_select_profile(const aether_power_observation_t *, aether_power_profile_t *);
#endif
