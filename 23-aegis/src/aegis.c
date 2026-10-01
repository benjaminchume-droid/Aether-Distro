#include "aether/aegis.h"
#include <string.h>

#define AETHER_AEGIS_MAX_RULES 512
static aether_aegis_rule_t rules[AETHER_AEGIS_MAX_RULES];
static size_t rule_count;

static int scope_matches(const char *rule_scope, const char *request_scope) {
    if (!rule_scope || !*rule_scope || !request_scope) return 1;
    return strcmp(rule_scope, request_scope) == 0;
}

aether_status_t aether_aegis_init(void) {
    rule_count = 0;
    return AETHER_OK;
}

aether_status_t aether_aegis_add_rule(const aether_aegis_rule_t *rule) {
    if (!rule || rule->subject_id == 0 || rule->resource == 0 ||
        rule->decision == 0) return AETHER_ERR_INVALID;
    if (rule_count >= AETHER_AEGIS_MAX_RULES) return AETHER_ERR_LIMIT;
    rules[rule_count++] = *rule;
    return AETHER_OK;
}

aether_status_t aether_aegis_remove_rule(aether_id_t subject_id,
                                          aether_aegis_resource_t resource,
                                          const char *scope) {
    for (size_t i = 0; i < rule_count; ++i) {
        if (rules[i].subject_id == subject_id && rules[i].resource == resource &&
            scope_matches(rules[i].scope, scope)) {
            rules[i] = rules[--rule_count];
            return AETHER_OK;
        }
    }
    return AETHER_ERR_NOT_FOUND;
}

aether_aegis_decision_t aether_aegis_evaluate(const aether_aegis_request_t *request) {
    if (!request || request->subject_id == 0 || request->resource == 0)
        return AETHER_AEGIS_DECISION_DENY;

    const aether_aegis_rule_t *best = NULL;
    for (size_t i = 0; i < rule_count; ++i) {
        const aether_aegis_rule_t *r = &rules[i];
        if (r->subject_id == request->subject_id &&
            r->resource == request->resource &&
            scope_matches(r->scope, request->scope) &&
            (!best || r->priority > best->priority)) {
            best = r;
        }
    }
    return best ? best->decision : AETHER_AEGIS_DECISION_DENY;
}

size_t aether_aegis_rule_count(void) { return rule_count; }
void aether_aegis_shutdown(void) { rule_count = 0; }
