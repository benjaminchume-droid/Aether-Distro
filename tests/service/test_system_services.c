#include <assert.h>
#include "aether/system_services.h"

int main(void) {
    assert(aether_system_services_init() == AETHER_OK);
    aether_system_service_status_t status;
    assert(aether_system_service_get(AETHER_SYSTEM_NETWORK, &status) == AETHER_OK);
    assert(status.available == 0);
    assert(aether_system_service_set_available(AETHER_SYSTEM_NETWORK, 1) == AETHER_OK);
    assert(aether_system_service_get(AETHER_SYSTEM_NETWORK, &status) == AETHER_OK);
    assert(status.available == 1);
    assert(aether_system_service_set_available((aether_system_service_kind_t)999, 1) == AETHER_ERR_NOT_FOUND);
    aether_system_services_shutdown();
    return 0;
}
