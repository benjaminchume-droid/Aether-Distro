#ifndef AETHER_SESSION_H
#define AETHER_SESSION_H
#include "types.h"
typedef enum { AETHER_SESSION_NONE=0,AETHER_SESSION_LOGIN,AETHER_SESSION_ACTIVE,AETHER_SESSION_LOCKED,AETHER_SESSION_LOGOUT } aether_session_state_t;
typedef struct { aether_id_t session_id; aether_id_t user_id; aether_session_state_t state; } aether_session_t;
aether_status_t aether_session_init(void);
aether_status_t aether_session_open(aether_id_t session_id,aether_id_t user_id);
aether_status_t aether_session_set_state(aether_session_state_t state);
aether_status_t aether_session_get(aether_session_t *out);
void aether_session_shutdown(void);
#endif