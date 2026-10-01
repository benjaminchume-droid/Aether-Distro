#ifndef AETHER_CPU_H
#define AETHER_CPU_H
#include <stddef.h>
#include <aether/types.h>
typedef struct { unsigned index,package,core,thread,online; } aether_cpu_t;
aether_status_t aether_cpu_scan(void); size_t aether_cpu_count(void); const aether_cpu_t*aether_cpu_get(size_t index);
#endif
