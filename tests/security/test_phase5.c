#include <assert.h>
#include <string.h>
#include "aether/identity_service.h"
#include "aether/account_service.h"
#include "aether/authentication_service.h"
#include "aether/permission_broker.h"
#include "aether/permission_subject.h"

static aether_status_t verify_test(aether_id_t user_id,uint64_t challenge_id,const void *credential,size_t size,void *context){
 (void)challenge_id;
 (void)user_id;
 (void)context;
 if(size!=4 || !credential) return AETHER_ERR_PERMISSION;
 return memcmp(credential,"pass",4)==0 ? AETHER_OK : AETHER_ERR_PERMISSION;
}

int main(void){
 aether_identity_record_t identity;
 assert(aether_identity_current(&identity)==AETHER_OK);
 assert(identity.user_id!=0 && identity.name[0]!='\0');

 aether_account_t account;
 assert(aether_account_current(&account)==AETHER_OK);
 assert(account.user_id==identity.user_id);

 assert(aether_authentication_init()==AETHER_OK);
 aether_auth_provider_t provider={.method=AETHER_AUTH_PASSWORD,.flags=0,.provider="test-provider",.verify=verify_test};
 assert(aether_authentication_register(&provider)==AETHER_OK);
 uint64_t challenge=0;
 aether_auth_request_t request={identity.user_id,AETHER_AUTH_PASSWORD,0};
 assert(aether_authentication_begin(&request,&challenge)==AETHER_OK);
 assert(challenge!=0);
 aether_auth_challenge_t state;
 assert(aether_authentication_get(challenge,&state)==AETHER_OK);
 assert(state.result==AETHER_AUTH_RESULT_PENDING);
 assert(aether_authentication_submit(challenge,"nope",4)==AETHER_OK);
 assert(aether_authentication_get(challenge,&state)==AETHER_OK);
 assert(state.result==AETHER_AUTH_RESULT_FAILURE);
 assert(aether_authentication_begin(&request,&challenge)==AETHER_OK);
 assert(aether_authentication_submit(challenge,"pass",4)==AETHER_OK);
 assert(aether_authentication_get(challenge,&state)==AETHER_OK);
 assert(state.result==AETHER_AUTH_RESULT_SUCCESS);
 assert(aether_authentication_complete(challenge,AETHER_AUTH_RESULT_FAILURE)==AETHER_ERR_STATE);
 assert(aether_authentication_get(challenge,&state)==AETHER_OK);
 assert(state.result==AETHER_AUTH_RESULT_SUCCESS);
 aether_authentication_shutdown();

 assert(aether_permission_subject_init()==AETHER_OK);
 aether_permission_subject_t subject={.subject_id=identity.user_id,.kind=AETHER_SUBJECT_USER,.uid=identity.uid,.name="current-user",.executable=""};
 assert(aether_permission_subject_register(&subject)==AETHER_OK);
 assert(aether_permission_init()==AETHER_OK);
 aether_permission_request_t denied={identity.user_id,AETHER_AEGIS_RESOURCE_PROCESS,"/bin/true"};
 assert(aether_permission_check(&denied)==AETHER_AEGIS_DECISION_DENY);
 assert(aether_permission_grant(identity.user_id,AETHER_AEGIS_RESOURCE_PROCESS,"/bin/true",10)==AETHER_OK);
 assert(aether_permission_check(&denied)==AETHER_AEGIS_DECISION_ALLOW);
 assert(aether_permission_revoke(identity.user_id,AETHER_AEGIS_RESOURCE_PROCESS,"/bin/true")==AETHER_OK);
 assert(aether_permission_check(&denied)==AETHER_AEGIS_DECISION_DENY);
 aether_permission_shutdown();
 aether_permission_subject_shutdown();
 return 0;
}
