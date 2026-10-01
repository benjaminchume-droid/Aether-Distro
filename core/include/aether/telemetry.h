#ifndef AETHER_TELEMETRY_H
#define AETHER_TELEMETRY_H
#include "types.h"
typedef struct {
 uint64_t timestamp;
 uint32_t cpu_percent;
 uint32_t gpu_percent;
 uint32_t memory_percent;
 uint32_t battery_percent;
 uint32_t temperature_c;
} aether_system_observation_t;
#endif
