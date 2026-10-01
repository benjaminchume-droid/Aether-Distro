#ifndef AETHER_NETWORK_H
#define AETHER_NETWORK_H
#include <stddef.h>
#include <aether/types.h>
typedef struct { char name[64]; unsigned up; unsigned wireless; } aether_network_interface_t;
aether_status_t aether_network_scan(void); size_t aether_network_count(void); const aether_network_interface_t*aether_network_get(size_t index);
#endif
