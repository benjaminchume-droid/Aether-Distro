#include <assert.h>
#include "aether/identity_service.h"
#include "aether/permission_broker.h"

int main(void){
 aether_identity_record_t identity;
 assert(aether_identity_current(&identity)==AETHER_OK);
 assert(identity.user_id!=0);
 assert(identity.name[0]!='\0');
 assert(aether_permission_init()==AETHER_OK);
 assert(aether_permission_check(&(aether_permission_request_t){identity.user_id,AETHER_AEGIS_NETWORK,"example"})==AETHER_AEGIS_DECISION_DENY);
 assert(aether_permission_grant(identity.user_id,AETHER_AEGIS_NETWORK,"example",10)==AETHER_OK);
 assert(aether_permission_check(&(aether_permission_request_t){identity.user_id,AETHER_AEGIS_NETWORK,"example"})==AETHER_AEGIS_DECISION_ALLOW);
 assert(aether_permission_revoke(identity.user_id,AETHER_AEGIS_NETWORK,"example")==AETHER_OK);
 assert(aether_permission_check(&(aether_permission_request_t){identity.user_id,AETHER_AEGIS_NETWORK,"example"})==AETHER_AEGIS_DECISION_DENY);
 aether_permission_shutdown();
 return 0;
}
