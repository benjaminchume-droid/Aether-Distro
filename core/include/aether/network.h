#ifndef AETHER_NETWORK_H
#define AETHER_NETWORK_H
#include "types.h"
typedef enum { AETHER_NET_ETHERNET=1, AETHER_NET_WIFI, AETHER_NET_BLUETOOTH } aether_network_kind_t;
typedef struct {
 aether_id_t id;
 aether_network_kind_t kind;
 const char *name;
 uint32_t flags;
} aether_network_device_t;
#endif
