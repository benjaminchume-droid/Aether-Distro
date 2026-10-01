#include "aether/identity.h"

aether_status_t aether_identity_validate(const aether_identity_t *i) {
    if (!i || i->user_id == 0 || !i->name || !i->home) return AETHER_ERR_INVALID;
    return AETHER_OK;
}
