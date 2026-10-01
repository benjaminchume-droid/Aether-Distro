#ifndef AETHER_NETWORK_SERVICE_H
#define AETHER_NETWORK_SERVICE_H
#include <stddef.h>
#include <aether/types.h>
typedef struct { char name[64]; unsigned up; unsigned wireless; } aether_network_state_t;
aether_status_t aether_network_service_refresh(void);
aether_status_t aether_network_state_count(size_t *count);
aether_status_t aether_network_state_get(size_t index,aether_network_state_t *out);
#endif