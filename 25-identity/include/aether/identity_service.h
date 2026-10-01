#ifndef AETHER_IDENTITY_SERVICE_H
#define AETHER_IDENTITY_SERVICE_H
#include <stdint.h>
#include "types.h"

typedef struct {
 aether_id_t user_id;
 char name[128];
 char home[256];
 char shell[256];
 uint32_t uid;
 uint32_t gid;
} aether_identity_record_t;

aether_status_t aether_identity_current(aether_identity_record_t *out);
aether_status_t aether_identity_lookup(uint32_t uid,aether_identity_record_t *out);
#endif
