#include <aether/device_manager.h>

#define AETHER_MAX_DEVICES 256

static aether_device_t devices[AETHER_MAX_DEVICES];
static size_t device_count_value;

aether_status_t aether_device_manager_init(void) {
    device_count_value = 0;
    return AETHER_OK;
}

aether_status_t aether_device_scan(void) {
    return AETHER_OK;
}

size_t aether_device_count(void) {
    return device_count_value;
}

const aether_device_t *aether_device_get(size_t index) {
    if (index >= device_count_value) return NULL;
    return &devices[index];
}
