#include <assert.h>
#include <stddef.h>
#include "aether/authentication_service.h"
#include "aether/pam_provider.h"

int main(void){
    assert(aether_authentication_init()==AETHER_OK);
    aether_status_t st=aether_pam_password_provider_register("login");
    assert(st==AETHER_OK || st==AETHER_ERR_UNAVAILABLE || st==AETHER_ERR_EXISTS);
    if(st==AETHER_OK){
        assert(aether_pam_password_provider_available()==1);
        uint64_t challenge=0;
        aether_auth_request_t request={.user_id=1,.method=AETHER_AUTH_PASSWORD,.challenge_id=0};
        assert(aether_authentication_begin(&request,&challenge)==AETHER_OK);
        assert(challenge!=0);
        assert(aether_authentication_submit(challenge,"",0)==AETHER_OK);
        aether_auth_challenge_t state;
        assert(aether_authentication_get(challenge,&state)==AETHER_OK);
        assert(state.result==AETHER_AUTH_RESULT_FAILURE || state.result==AETHER_AUTH_RESULT_UNAVAILABLE);
        aether_pam_password_provider_shutdown();
    }
    aether_authentication_shutdown();
    return 0;
}
