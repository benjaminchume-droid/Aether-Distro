#ifndef AETHER_FIRMWARE_H
#define AETHER_FIRMWARE_H
#include <stddef.h>
#include <aether/types.h>
typedef struct { char name[128]; char path[256]; } aether_firmware_entry_t;
aether_status_t aether_firmware_scan(void); size_t aether_firmware_count(void); const aether_firmware_entry_t*aether_firmware_get(size_t index);
#endif
