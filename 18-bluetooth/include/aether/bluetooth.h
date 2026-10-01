#ifndef AETHER_BLUETOOTH_H
#define AETHER_BLUETOOTH_H
#include <stddef.h>
#include <aether/types.h>
typedef struct { char name[64]; char address[32]; } aether_bluetooth_adapter_t;
aether_status_t aether_bluetooth_scan(void); size_t aether_bluetooth_count(void); const aether_bluetooth_adapter_t*aether_bluetooth_get(size_t index);
#endif
