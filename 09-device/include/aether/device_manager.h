#ifndef AETHER_DEVICE_MANAGER_H
#define AETHER_DEVICE_MANAGER_H

#include <stddef.h>
#include <aether/device.h>
#include <aether/types.h>

aether_status_t aether_device_manager_init(void);
aether_status_t aether_device_scan(void);
size_t aether_device_count(void);
const aether_device_t *aether_device_get(size_t index);

#endif
