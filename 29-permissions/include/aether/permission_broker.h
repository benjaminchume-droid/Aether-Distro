#ifndef AETHER_PERMISSION_BROKER_H
#define AETHER_PERMISSION_BROKER_H
#include <stddef.h>
#include <aether/types.h>
#include <aether/aegis.h>

typedef struct {
 aether_id_t subject_id;
 aether_aegis_resource_t resource;
 const char *scope;
} aether_permission_request_t;

aether_status_t aether_permission_init(void);
aether_status_t aether_permission_grant(aether_id_t subject,aether_aegis_resource_t resource,const char *scope,unsigned priority);
aether_status_t aether_permission_grant_access(aether_id_t subject,aether_aegis_resource_t resource,const char *scope,unsigned priority,uint32_t access);
aether_status_t aether_permission_revoke(aether_id_t subject,aether_aegis_resource_t resource,const char *scope);
aether_status_t aether_permission_revoke_access(aether_id_t subject,aether_aegis_resource_t resource,const char *scope,uint32_t access);
aether_aegis_decision_t aether_permission_check(const aether_permission_request_t *request);
void aether_permission_shutdown(void);
#endif
