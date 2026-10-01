#include "aether/permission_broker.h"
#include "../include/aether/permission_subject.h"

aether_status_t aether_permission_init(void){return aether_aegis_init();}

aether_status_t aether_permission_grant(aether_id_t subject,aether_aegis_resource_t resource,const char *scope,unsigned priority){
 if(!subject)return AETHER_ERR_INVALID;
 if(!aether_permission_subject_find(subject))return AETHER_ERR_NOT_FOUND;
 aether_aegis_rule_t rule={subject,resource,AETHER_AEGIS_DECISION_ALLOW,scope,priority};
 return aether_aegis_add_rule(&rule);
}

aether_status_t aether_permission_revoke(aether_id_t subject,aether_aegis_resource_t resource,const char *scope){
 if(!subject)return AETHER_ERR_INVALID;
 if(!aether_permission_subject_find(subject))return AETHER_ERR_NOT_FOUND;
 return aether_aegis_remove_rule(subject,resource,scope);
}

aether_aegis_decision_t aether_permission_check(const aether_permission_request_t *request){
 if(!request||!request->subject_id)return AETHER_AEGIS_DECISION_DENY;
 aether_aegis_request_t r={request->subject_id,request->resource,request->scope};
 return aether_aegis_evaluate(&r);
}

void aether_permission_shutdown(void){aether_aegis_shutdown();}
