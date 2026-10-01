#define _GNU_SOURCE
#include <assert.h>
#include "aether/aegis.h"

int main(void){
    assert(aether_aegis_init()==AETHER_OK);

    aether_aegis_rule_t read_rule={
        .subject_id=77,
        .resource=AETHER_AEGIS_RESOURCE_FILESYSTEM,
        .decision=AETHER_AEGIS_DECISION_ALLOW,
        .scope="/data",
        .priority=10,
        .access=AETHER_AEGIS_ACCESS_READ
    };
    assert(aether_aegis_add_rule(&read_rule)==AETHER_OK);

    assert(aether_aegis_evaluate(&(aether_aegis_request_t){
        .subject_id=77,.resource=AETHER_AEGIS_RESOURCE_FILESYSTEM,.scope="/data",
        .access=AETHER_AEGIS_ACCESS_READ
    })==AETHER_AEGIS_DECISION_ALLOW);

    assert(aether_aegis_evaluate(&(aether_aegis_request_t){
        .subject_id=77,.resource=AETHER_AEGIS_RESOURCE_FILESYSTEM,.scope="/data",
        .access=AETHER_AEGIS_ACCESS_WRITE
    })==AETHER_AEGIS_DECISION_DENY);

    aether_aegis_rule_t write_rule={
        .subject_id=77,
        .resource=AETHER_AEGIS_RESOURCE_FILESYSTEM,
        .decision=AETHER_AEGIS_DECISION_ALLOW,
        .scope="/data",
        .priority=20,
        .access=AETHER_AEGIS_ACCESS_WRITE
    };
    assert(aether_aegis_add_rule(&write_rule)==AETHER_OK);

    assert(aether_aegis_evaluate(&(aether_aegis_request_t){
        .subject_id=77,.resource=AETHER_AEGIS_RESOURCE_FILESYSTEM,.scope="/data",
        .access=AETHER_AEGIS_ACCESS_WRITE
    })==AETHER_AEGIS_DECISION_ALLOW);

    assert(aether_aegis_evaluate(&(aether_aegis_request_t){
        .subject_id=77,.resource=AETHER_AEGIS_RESOURCE_FILESYSTEM,.scope="/data",
        .access=AETHER_AEGIS_ACCESS_READ|AETHER_AEGIS_ACCESS_WRITE
    })==AETHER_AEGIS_DECISION_DENY);

    assert(aether_aegis_remove_rule_access(77,AETHER_AEGIS_RESOURCE_FILESYSTEM,"/data",AETHER_AEGIS_ACCESS_WRITE)==AETHER_OK);
    assert(aether_aegis_evaluate(&(aether_aegis_request_t){
        .subject_id=77,.resource=AETHER_AEGIS_RESOURCE_FILESYSTEM,.scope="/data",
        .access=AETHER_AEGIS_ACCESS_WRITE
    })==AETHER_AEGIS_DECISION_DENY);

    aether_aegis_shutdown();
    return 0;
}
