#ifndef AETHER_GPU_H
#define AETHER_GPU_H
#include <stddef.h>
#include <aether/types.h>
typedef struct { unsigned id; char name[128],vendor[64],driver[64],pci_address[32]; } aether_gpu_t;
aether_status_t aether_gpu_scan(void); size_t aether_gpu_count(void); const aether_gpu_t*aether_gpu_get(size_t index);
#endif
