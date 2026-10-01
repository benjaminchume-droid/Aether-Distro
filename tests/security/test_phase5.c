#include <assert.h>
#include "aether/identity_service.h"
#include "aether/account_service.h"
#include "aether/authentication_service.h"
#include "aether/permission_broker.h"

int main(void){
 aether_identity_record_t identity;
 assert(aether_identity_current(&identity)==AETHER_OK);
 assert(identity.user_id!=0 && identity.name[0]!='\0');

 aether_account_t account;
 assert(aether_account_current(&account)==AETHER_OK);
 assert(account.user_id==identity.user_id);

 assert(aether_authentication_init()==AETHER_OK);
 aether_auth_provider_t provider={AETHER_AUTH_PASSWORD,0,"host-password-provider"};
 assert(aether_authentication_register(&provider)==AETHER_OK);
 uint64_t challenge=0;
 aether_auth_request_t request={identity.user_id,AETHER_AUTH_PASSWORD,0};
 assert(aether_authentication_begin(&request,&challenge)==AETHER_OK);
 assert(challenge!=0);
 aether_auth_challenge_t state;
 assert(aether_authentication_get(challenge,&state)==AETHER_OK);
 assert(state.result==AETHER_AUTH_RESULT_PENDING);
 assert(aether_authentication_complete(challenge,AETHER_AUTH_RESULT_FAILURE)==AETHER_OK);
 assert(aether_authentication_get(challenge,&state)==AETHER_OK);
 assert(state.result==AETHER_AUTH_RESULT_FAILURE);
 aether_authentication_shutdown();

 assert(aether_permission_init()==AETHER_OK);
 aether_permission_request_t denied={identity.user_id,AETHER_AEGIS_RESOURCE_PROCESS,"/bin/true"};
 assert(aether_permission_check(&denied)==AETHER_AEGIS_DECISION_DENY);
 assert(aether_permission_grant(identity.user_id,AETHER_AEGIS_RESOURCE_PROCESS,"/bin/true",10)==AETHER_OK);
 assert(aether_permission_check(&denied)==AETHER_AEGIS_DECISION_ALLOW);
 assert(aether_permission_revoke(identity.user_id,AETHER_AEGIS_RESOURCE_PROCESS,"/bin/true")==AETHER_OK);
 assert(aether_permission_check(&denied)==AETHER_AEGIS_DECISION_DENY);
 aether_permission_shutdown();
 return 0;
}
