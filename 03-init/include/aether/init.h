#ifndef AETHER_INIT_H
#define AETHER_INIT_H

#include <aether/types.h>

aether_status_t aether_init_early(void);
aether_status_t aether_init_mounts(void);
aether_status_t aether_init_services(void);
aether_status_t aether_init_shutdown(void);

#endif
