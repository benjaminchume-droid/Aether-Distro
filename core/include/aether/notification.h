#ifndef AETHER_NOTIFICATION_H
#define AETHER_NOTIFICATION_H
#include <stddef.h>
#include "types.h"
typedef struct { aether_id_t id; aether_id_t app_id; const char *title; const char *body; } aether_notification_t;
aether_status_t aether_notification_init(void);
aether_status_t aether_notification_post(aether_id_t app_id,const char *title,const char *body,aether_id_t *id);
aether_status_t aether_notification_get(size_t index,aether_notification_t *out);
size_t aether_notification_count(void);
void aether_notification_shutdown(void);
#endif