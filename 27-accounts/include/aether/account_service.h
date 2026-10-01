#ifndef AETHER_ACCOUNT_SERVICE_H
#define AETHER_ACCOUNT_SERVICE_H

#include <stddef.h>
#include <stdint.h>
#include <aether/types.h>

typedef struct {
    aether_id_t user_id;
    uint32_t uid;
    uint32_t gid;
    char name[128];
    char home[256];
    char shell[256];
    uint32_t flags;
} aether_account_t;

enum {
    AETHER_ACCOUNT_SYSTEM = 1u << 0,
    AETHER_ACCOUNT_LOCKED = 1u << 1,
    AETHER_ACCOUNT_NO_LOGIN = 1u << 2
};

aether_status_t aether_account_current(aether_account_t *out);
aether_status_t aether_account_lookup_uid(uint32_t uid, aether_account_t *out);
aether_status_t aether_account_lookup_name(const char *name, aether_account_t *out);
aether_status_t aether_account_refresh(void);
size_t aether_account_count(void);
aether_status_t aether_account_get(size_t index, aether_account_t *out);
#endif
