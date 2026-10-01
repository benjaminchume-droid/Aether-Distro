#ifndef AETHER_AEGIS_H
#define AETHER_AEGIS_H
#include <stddef.h>
#include <stdint.h>
#include <aether/types.h>

typedef enum {
    AETHER_AEGIS_DECISION_ALLOW = 1,
    AETHER_AEGIS_DECISION_DENY,
    AETHER_AEGIS_DECISION_AUDIT
} aether_aegis_decision_t;

typedef enum {
    AETHER_AEGIS_RESOURCE_FILESYSTEM = 1,
    AETHER_AEGIS_RESOURCE_NETWORK,
    AETHER_AEGIS_RESOURCE_CAMERA,
    AETHER_AEGIS_RESOURCE_MICROPHONE,
    AETHER_AEGIS_RESOURCE_BLUETOOTH,
    AETHER_AEGIS_RESOURCE_USB,
    AETHER_AEGIS_RESOURCE_LOCATION,
    AETHER_AEGIS_RESOURCE_IPC,
    AETHER_AEGIS_RESOURCE_PROCESS
} aether_aegis_resource_t;

typedef struct {
    aether_id_t subject_id;
    aether_aegis_resource_t resource;
    aether_aegis_decision_t decision;
    const char *scope;
    uint32_t priority;
} aether_aegis_rule_t;

typedef struct {
    aether_id_t subject_id;
    aether_aegis_resource_t resource;
    const char *scope;
} aether_aegis_request_t;

aether_status_t aether_aegis_init(void);
aether_status_t aether_aegis_add_rule(const aether_aegis_rule_t *rule);
aether_status_t aether_aegis_remove_rule(aether_id_t subject_id, aether_aegis_resource_t resource, const char *scope);
aether_aegis_decision_t aether_aegis_evaluate(const aether_aegis_request_t *request);
size_t aether_aegis_rule_count(void);
aether_status_t aether_aegis_rule_get(size_t index,aether_aegis_rule_t *out);
aether_status_t aether_aegis_save(const char *path);
aether_status_t aether_aegis_load(const char *path);
void aether_aegis_shutdown(void);
#endif
