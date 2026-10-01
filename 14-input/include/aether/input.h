#ifndef AETHER_INPUT_H
#define AETHER_INPUT_H
#include <stddef.h>
#include <aether/types.h>
typedef enum { AETHER_INPUT_KEYBOARD=1,AETHER_INPUT_POINTER,AETHER_INPUT_TOUCH,AETHER_INPUT_GAMEPAD,AETHER_INPUT_OTHER } aether_input_kind_t;
typedef struct { unsigned id; aether_input_kind_t kind; char name[128]; char path[256]; } aether_input_device_t;
aether_status_t aether_input_scan(void); size_t aether_input_count(void); const aether_input_device_t*aether_input_get(size_t index);
#endif
