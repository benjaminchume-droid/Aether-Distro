#ifndef AETHER_IDENTITY_H
#define AETHER_IDENTITY_H
#include "types.h"
typedef struct { aether_id_t user_id; const char *name; const char *home; uint32_t flags; } aether_identity_t;
aether_status_t aether_identity_validate(const aether_identity_t *);
#endif
