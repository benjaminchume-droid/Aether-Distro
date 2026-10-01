#define _GNU_SOURCE
#include "aether/aegis.h"
#include <stdio.h>
#include <stdlib.h>
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
        scope_storage[i][0]='\0';
        rules[i].scope=NULL;
    }
    return AETHER_OK;
}

aether_status_t aether_aegis_remove_rule(aether_id_t subject_id,aether_aegis_resource_t resource,const char *scope){
    for(size_t i=0;i<rule_count;i++){
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
    for(size_t i=0;i<rule_count;i++){
        const aether_aegis_rule_t *r=&rules[i];
        if(r->subject_id==request->subject_id && r->resource==request->resource &&
           scope_matches(r->scope,request->scope) &&
           (!best || r->priority>best->priority))
            best=r;
    }
    return best ? best->decision : AETHER_AEGIS_DECISION_DENY;
}

size_t aether_aegis_rule_count(void){return rule_count;}

aether_status_t aether_aegis_rule_get(size_t index,aether_aegis_rule_t *out){
    if(!out) return AETHER_ERR_INVALID;
    if(index>=rule_count) return AETHER_ERR_NOT_FOUND;
    *out=rules[index];
    return AETHER_OK;
}

aether_status_t aether_aegis_save(const char *path){
    if(!path || !*path) return AETHER_ERR_INVALID;
    FILE *f=fopen(path,"w");
    if(!f) return AETHER_ERR_IO;

    for(size_t i=0;i<rule_count;i++){
        const aether_aegis_rule_t *r=&rules[i];
        const char *scope=(r->scope && *r->scope) ? r->scope : "-";
        if(strchr(scope,'\n') || strchr(scope,'\r') || strchr(scope,'\t')){
            fclose(f);
            return AETHER_ERR_INVALID;
        }
        if(fprintf(f,"%llu %u %u %u %s\n",
                   (unsigned long long)r->subject_id,
                   (unsigned)r->resource,
                   (unsigned)r->decision,
                   (unsigned)r->priority,
                   scope)<0){
            fclose(f);
            return AETHER_ERR_IO;
        }
    }
    return fclose(f)==0 ? AETHER_OK : AETHER_ERR_IO;
}

aether_status_t aether_aegis_load(const char *path){
    if(!path || !*path) return AETHER_ERR_INVALID;
    FILE *f=fopen(path,"r");
    if(!f) return AETHER_ERR_IO;

    aether_aegis_rule_t loaded[AETHER_AEGIS_MAX_RULES];
    char scopes[AETHER_AEGIS_MAX_RULES][AETHER_AEGIS_SCOPE_MAX];
    size_t loaded_count=0;
    memset(loaded,0,sizeof(loaded));
    memset(scopes,0,sizeof(scopes));

    char line[512];
    while(fgets(line,sizeof(line),f)){
        if(line[0]=='#' || line[0]=='\n' || line[0]=='\r') continue;

        char *saveptr=NULL;
        char *subject_text=strtok_r(line," \t\r\n",&saveptr);
        char *resource_text=strtok_r(NULL," \t\r\n",&saveptr);
        char *decision_text=strtok_r(NULL," \t\r\n",&saveptr);
        char *priority_text=strtok_r(NULL," \t\r\n",&saveptr);
        char *scope_text=strtok_r(NULL," \t\r\n",&saveptr);
        if(!subject_text || !resource_text || !decision_text || !priority_text || !scope_text){
            fclose(f);
            return AETHER_ERR_INVALID;
        }
        if(loaded_count>=AETHER_AEGIS_MAX_RULES){
            fclose(f);
            return AETHER_ERR_LIMIT;
        }

        char *end=NULL;
        unsigned long long subject=strtoull(subject_text,&end,10);
        if(!end || *end){fclose(f);return AETHER_ERR_INVALID;}
        unsigned long resource=strtoul(resource_text,&end,10);
        if(!end || *end){fclose(f);return AETHER_ERR_INVALID;}
        unsigned long decision=strtoul(decision_text,&end,10);
        if(!end || *end){fclose(f);return AETHER_ERR_INVALID;}
        unsigned long priority=strtoul(priority_text,&end,10);
        if(!end || *end){fclose(f);return AETHER_ERR_INVALID;}

        if(subject==0 || resource==0 ||
           decision<AETHER_AEGIS_DECISION_ALLOW || decision>AETHER_AEGIS_DECISION_AUDIT ||
           priority>UINT32_MAX || strlen(scope_text)>=AETHER_AEGIS_SCOPE_MAX){
            fclose(f);
            return AETHER_ERR_INVALID;
        }

        loaded[loaded_count].subject_id=(aether_id_t)subject;
        loaded[loaded_count].resource=(aether_aegis_resource_t)resource;
        loaded[loaded_count].decision=(aether_aegis_decision_t)decision;
        loaded[loaded_count].priority=(uint32_t)priority;
        if(strcmp(scope_text,"-")!=0){
            snprintf(scopes[loaded_count],sizeof(scopes[loaded_count]),"%s",scope_text);
            loaded[loaded_count].scope=scopes[loaded_count];
        }
        loaded_count++;
    }

    if(ferror(f)){fclose(f);return AETHER_ERR_IO;}
    fclose(f);

    aether_aegis_init();
    for(size_t i=0;i<loaded_count;i++){
        aether_status_t st=aether_aegis_add_rule(&loaded[i]);
        if(st!=AETHER_OK){
            aether_aegis_shutdown();
            return st;
        }
    }
    return AETHER_OK;
}

void aether_aegis_shutdown(void){
    rule_count=0;
    memset(rules,0,sizeof(rules));
    memset(scope_storage,0,sizeof(scope_storage));
}
