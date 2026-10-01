#ifndef AETHER_TPM_SERVICE_H
#define AETHER_TPM_SERVICE_H

#include <stddef.h>
#include <stdint.h>
#include "types.h"

typedef struct {
    char device_path[128];
    char description[256];
    uint32_t major_version;
    uint32_t minor_version;
    unsigned resource_manager;
    unsigned available;
} aether_tpm_device_t;

aether_status_t aether_tpm_refresh(void);
size_t aether_tpm_count(void);
aether_status_t aether_tpm_get(size_t index,aether_tpm_device_t *out);
int aether_tpm_available(void);
#endif
