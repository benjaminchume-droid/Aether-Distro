#ifndef AETHER_DISPLAY_H
#define AETHER_DISPLAY_H
#include "types.h"
typedef struct {
 aether_id_t id;
 uint32_t width;
 uint32_t height;
 uint32_t refresh_millihz;
 uint32_t flags;
} aether_display_mode_t;
#endif
