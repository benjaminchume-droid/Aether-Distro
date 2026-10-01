#include "aether/power.h"

aether_status_t aether_power_select_profile(
    const aether_power_observation_t *o, aether_power_profile_t *p) {
    if (!o || !p) return AETHER_ERR_INVALID;
    if (o->foreground_latency > 80 || o->gpu_utilization > 80 || o->cpu_utilization > 85)
        *p = AETHER_POWER_PERFORMANCE;
    else if (o->battery_percent < 15)
        *p = AETHER_POWER_BATTERY_SAVER;
    else if (o->cpu_utilization < 10 && o->gpu_utilization < 10)
        *p = AETHER_POWER_EFFICIENCY;
    else
        *p = AETHER_POWER_BALANCED;
    return AETHER_OK;
}
