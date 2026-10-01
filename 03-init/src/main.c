#include <unistd.h>
#include <aether/init.h>
#include <aether/service_manager.h>
#include <aether/capability_registry.h>
#include <aether/device_manager.h>

int main(void) {
    if (aether_init_early() != AETHER_OK) return 1;
    if (aether_init_mounts() != AETHER_OK) return 1;
    if (aether_capability_registry_init() != AETHER_OK) return 1;
    if (aether_device_manager_init() != AETHER_OK) return 1;
    if (aether_device_scan() != AETHER_OK) return 1;
    if (aether_service_manager_init() != AETHER_OK) return 1;
    if (aether_init_services() != AETHER_OK) return 1;
    for (;;) pause();
}
