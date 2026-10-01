#include "aether/session.h"
#include "aether/event_bus.h"
static aether_session_t session;
aether_status_t aether_session_init(void){session=(aether_session_t){0};return AETHER_OK;}
aether_status_t aether_session_open(aether_id_t sid,aether_id_t uid){if(!sid||!uid)return AETHER_ERR_INVALID;session.session_id=sid;session.user_id=uid;return aether_session_set_state(AETHER_SESSION_ACTIVE);}
aether_status_t aether_session_set_state(aether_session_state_t state){if(state< AETHER_SESSION_NONE||state>AETHER_SESSION_LOGOUT)return AETHER_ERR_INVALID;if(session.state==state)return AETHER_OK;session.state=state;aether_event_t e={AETHER_EVENT_SESSION_CHANGED,session.session_id,&session,sizeof(session)};return aether_event_publish(&e);}
aether_status_t aether_session_get(aether_session_t*out){if(!out)return AETHER_ERR_INVALID;*out=session;return AETHER_OK;}
void aether_session_shutdown(void){session=(aether_session_t){0};}
