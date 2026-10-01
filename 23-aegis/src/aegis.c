#include "aether/aegis.h"
#include <string.h>

#define AETHER_AEGIS_MAX_RULES 512
#define AETHER_AEGIS_SCOPE_MAX 256

static aether_aegis_rule_t rules[AETHER_AEGIS_MAX_RULES];
static char scope_storage[AETHER_AEGIS_MAX_RULES][AETHER_AEGIS_SCOPE_MAX];
static size_t rule_count;

static int scope_matches(const char *rule_scope,const char *request_scope){
    if(!rule_scope || !*rule_scope) return 1;
    if(!request_scope || !*request_scope) return 0;
    return strcmp(rule_scope,request_scope)==0;
}

aether_status_t aether_aegis_init(void){
    rule_count=0;
    memset(rules,0,sizeof(rules));
    memset(scope_storage,0,sizeof(scope_storage));
    return AETHER_OK;
}

aether_status_t aether_aegis_add_rule(const aether_aegis_rule_t *rule){
    if(!rule || rule->subject_id==0 || rule->resource==0 ||
       rule->decision<AETHER_AEGIS_DECISION_ALLOW || rule->decision>AETHER_AEGIS_DECISION_AUDIT)
        return AETHER_ERR_INVALID;
    if(rule_count>=AETHER_AEGIS_MAX_RULES) return AETHER_ERR_LIMIT;
    if(rule->scope && strlen(rule->scope)>=AETHER_AEGIS_SCOPE_MAX) return AETHER_ERR_LIMIT;

    size_t i=rule_count++;
    rules[i]=*rule;
    if(rule->scope && *rule->scope){
        snprintf(scope_storage[i],sizeof(scope_storage[i]),"%s",rule->scope);
        rules[i].scope=scope_storage[i];
    }else{
        rules[i].scope=NULL;
        scope_storage[i][0]='\0';
    }
    return AETHER_OK;
}

aether_status_t aether_aegis_remove_rule(aether_id_t subject_id,aether_aegis_resource_t resource,const char *scope){
    for(size_t i=0;i<rule_count;++i){
        if(rules[i].subject_id==subject_id && rules[i].resource==resource &&
           scope_matches(rules[i].scope,scope)){
            size_t last=--rule_count;
            if(i!=last){
                rules[i]=rules[last];
                memcpy(scope_storage[i],scope_storage[last],sizeof(scope_storage[i]));
                rules[i].scope=scope_storage[i][0] ? scope_storage[i] : NULL;
            }
            memset(&rules[last],0,sizeof(rules[last]));
            memset(scope_storage[last],0,sizeof(scope_storage[last]));
            return AETHER_OK;
        }
    }
    return AETHER_ERR_NOT_FOUND;
}

aether_aegis_decision_t aether_aegis_evaluate(const aether_aegis_request_t *request){
    if(!request || !request->subject_id || !request->resource)
        return AETHER_AEGIS_DECISION_DENY;

    const aether_aegis_rule_t *best=NULL;
    for(size_t i=0;i<rule_count;++i){
        const aether_aegis_rule_t *r=&rules[i];
        if(r->subject_id==request->subject_id && r->resource==request->resource &&
           scope_matches(r->scope,request->scope) &&
           (!best || r->priority>best->priority))
            best=r;
    }
    return best ? best->decision : AETHER_AEGIS_DECISION_DENY;
}

size_t aether_aegis_rule_count(void){return rule_count;}
void aether_aegis_shutdown(void){rule_count=0;memset(rules,0,sizeof(rules));memset(scope_storage,0,sizeof(scope_storage));}
