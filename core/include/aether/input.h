#ifndef AETHER_INPUT_H
#define AETHER_INPUT_H
#include "types.h"
typedef enum { AETHER_INPUT_KEYBOARD=1, AETHER_INPUT_POINTER, AETHER_INPUT_TOUCH, AETHER_INPUT_GAMEPAD } aether_input_kind_t;
typedef struct {
 aether_id_t device_id;
 aether_input_kind_t kind;
 const char *name;
 uint32_t flags;
} aether_input_device_t;
#endif
