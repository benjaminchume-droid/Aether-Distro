#ifndef AETHER_CAMERA_H
#define AETHER_CAMERA_H
#include <stddef.h>
#include <aether/types.h>
typedef struct { unsigned id; char path[128]; char name[128]; } aether_camera_t;
aether_status_t aether_camera_scan(void); size_t aether_camera_count(void); const aether_camera_t*aether_camera_get(size_t index);
#endif
