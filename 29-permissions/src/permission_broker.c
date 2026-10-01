#include "aether/permission_broker.h"
#include "../include/aether/permission_subject.h"

static aether_status_t check_subject(aether_id_t subject){
    if(!subject) return AETHER_ERR_INVALID;
    if(!aether_permission_subject_find(subject)) return AETHER_ERR_NOT_FOUND;
    return AETHER_OK;
}

aether_status_t aether_permission_init(void){return aether_aegis_init();}

aether_status_t aether_permission_grant_access(aether_id_t subject,aether_aegis_resource_t resource,
                                                 const char *scope,unsigned priority,uint32_t access){
    aether_status_t st=check_subject(subject);
    if(st!=AETHER_OK) return st;
    aether_aegis_rule_t rule={
        .subject_id=subject,.resource=resource,.decision=AETHER_AEGIS_DECISION_ALLOW,
        .scope=scope,.priority=priority,.access=access
    };
    return aether_aegis_add_rule(&rule);
}

aether_status_t aether_permission_grant(aether_id_t subject,aether_aegis_resource_t resource,
                                         const char *scope,unsigned priority){
    return aether_permission_grant_access(subject,resource,scope,priority,0);
}

aether_status_t aether_permission_revoke_access(aether_id_t subject,aether_aegis_resource_t resource,
                                                 const char *scope,uint32_t access){
    aether_status_t st=check_subject(subject);
    if(st!=AETHER_OK) return st;
    return aether_aegis_remove_rule_access(subject,resource,scope,access);
}

aether_status_t aether_permission_revoke(aether_id_t subject,aether_aegis_resource_t resource,
                                         const char *scope){
    return aether_permission_revoke_access(subject,resource,scope,0);
}

aether_aegis_decision_t aether_permission_check(const aether_permission_request_t *request){
    if(!request || !request->subject_id) return AETHER_AEGIS_DECISION_DENY;
    aether_aegis_request_t r={
        .subject_id=request->subject_id,
        .resource=request->resource,
        .scope=request->scope,
        .access=request->access
    };
    return aether_aegis_evaluate(&r);
}

void aether_permission_shutdown(void){aether_aegis_shutdown();}
