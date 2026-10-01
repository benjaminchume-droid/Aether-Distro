#include "../include/aether/network_service.h"
#include "../include/aether/network.h"
#include <stdio.h>

aether_status_t aether_network_service_refresh(void){return aether_network_scan();}
aether_status_t aether_network_state_count(size_t*out){
    if(!out) return AETHER_ERR_INVALID;
    *out=aether_network_count();
    return AETHER_OK;
}
aether_status_t aether_network_state_get(size_t i,aether_network_state_t*out){
    if(!out) return AETHER_ERR_INVALID;
    const aether_network_interface_t*n=aether_network_get(i);
    if(!n) return AETHER_ERR_NOT_FOUND;
    out->up=n->up;
    out->wireless=n->wireless;
    snprintf(out->name,sizeof(out->name),"%s",n->name);
    return AETHER_OK;
}
