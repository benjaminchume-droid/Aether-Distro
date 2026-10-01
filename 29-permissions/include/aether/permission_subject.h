#ifndef AETHER_PERMISSION_SUBJECT_H
#define AETHER_PERMISSION_SUBJECT_H

#include <stddef.h>
#include <stdint.h>
#include <aether/types.h>

typedef enum {
    AETHER_SUBJECT_USER=1,
    AETHER_SUBJECT_APP,
    AETHER_SUBJECT_SERVICE,
    AETHER_SUBJECT_WORKLOAD
} aether_subject_kind_t;

typedef struct {
    aether_id_t subject_id;
    aether_subject_kind_t kind;
    uint32_t uid;
    char name[128];
    char executable[256];
    uint32_t flags;
} aether_permission_subject_t;

aether_status_t aether_permission_subject_init(void);
aether_status_t aether_permission_subject_register(const aether_permission_subject_t *subject);
aether_status_t aether_permission_subject_unregister(aether_id_t subject_id);
const aether_permission_subject_t *aether_permission_subject_find(aether_id_t subject_id);
size_t aether_permission_subject_count(void);
aether_status_t aether_permission_subject_get(size_t index,aether_permission_subject_t *out);
void aether_permission_subject_shutdown(void);
#endif
