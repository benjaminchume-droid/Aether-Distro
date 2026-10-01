#include "aether/event_bus.h"

#define AETHER_EVENT_MAX_SUBSCRIPTIONS 256

static aether_event_subscription_t subscriptions[AETHER_EVENT_MAX_SUBSCRIPTIONS];
static size_t subscription_count;

aether_status_t aether_event_bus_init(void) {
    subscription_count = 0;
    return AETHER_OK;
}

aether_status_t aether_event_subscribe(const aether_event_subscription_t *subscription) {
    if (!subscription || !subscription->handler) return AETHER_ERR_INVALID;
    if (subscription_count >= AETHER_EVENT_MAX_SUBSCRIPTIONS) return AETHER_ERR_LIMIT;
    subscriptions[subscription_count++] = *subscription;
    return AETHER_OK;
}

aether_status_t aether_event_publish(const aether_event_t *event) {
    if (!event) return AETHER_ERR_INVALID;
    for (size_t i = 0; i < subscription_count; ++i)
        if (subscriptions[i].kind == event->kind)
            subscriptions[i].handler(event, subscriptions[i].context);
    return AETHER_OK;
}

void aether_event_bus_shutdown(void) {
    subscription_count = 0;
}
