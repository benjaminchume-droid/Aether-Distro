#ifndef AETHER_POWER_H
#define AETHER_POWER_H
#include <aether/types.h>
typedef struct { unsigned ac_online; unsigned suspended; } aether_power_state_t;
aether_status_t aether_power_state_read(aether_power_state_t *state);
#endif
