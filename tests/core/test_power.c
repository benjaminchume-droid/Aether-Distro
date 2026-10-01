#include <assert.h>
#include "aether/power.h"

int main(void) {
    aether_power_observation_t idle = {
        .cpu_utilization=2, .gpu_utilization=1, .thermal_state=0,
        .battery_percent=80, .display_activity=1, .foreground_latency=5
    };
    aether_power_profile_t profile = AETHER_POWER_BALANCED;
    assert(aether_power_select_profile(&idle, &profile) == AETHER_OK);
    assert(profile == AETHER_POWER_EFFICIENCY);
    return 0;
}
