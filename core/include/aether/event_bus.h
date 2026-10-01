#ifndef AETHER_EVENT_BUS_H
#define AETHER_EVENT_BUS_H

#include <stddef.h>
#include "event.h"

typedef void (*aether_event_handler_t)(const aether_event_t *event, void *context);

typedef struct {
    aether_event_kind_t kind;
    aether_event_handler_t handler;
    void *context;
} aether_event_subscription_t;

aether_status_t aether_event_bus_init(void);
aether_status_t aether_event_subscribe(const aether_event_subscription_t *subscription);
aether_status_t aether_event_publish(const aether_event_t *event);
void aether_event_bus_shutdown(void);

#endif
