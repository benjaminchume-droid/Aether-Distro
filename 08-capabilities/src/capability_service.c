#include <aether/capability_service.h>
#include <aether/capability_registry.h>
#include "../../10-cpu/include/aether/cpu.h"
#include "../../11-gpu/include/aether/gpu.h"
#include "../../12-display/include/aether/display.h"
#include "../../13-audio/include/aether/audio.h"
#include "../../15-camera/include/aether/camera.h"
#include "../../18-bluetooth/include/aether/bluetooth.h"
#include "../../17-network/include/aether/network.h"
#include "../../24-tpm/include/aether/tpm_service.h"
#include "../../21-battery/include/aether/battery.h"
#include <stddef.h>
#include <stdio.h>

static aether_id_t capability_device_id(aether_capability_kind_t kind,size_t index){
    return ((aether_id_t)(uint32_t)kind<<32) | (aether_id_t)(index+1);
}

static aether_status_t add(aether_capability_kind_t kind,size_t index,const char *name,const char *provider){
    aether_capability_record_t r={0};
    r.kind=kind;
    r.device_id=capability_device_id(kind,index);
    r.name=name;
    r.provider=provider;
    r.available=1;
    return aether_capability_register(&r);
}

aether_status_t aether_capability_service_init(void){
    return aether_capability_registry_init();
}

aether_status_t aether_capability_refresh(void){
    aether_status_t first_error=AETHER_OK;
    aether_capability_registry_init();

    if(aether_cpu_scan()==AETHER_OK){
        for(size_t i=0,n=aether_cpu_count();i<n;i++){
            const aether_cpu_t *d=aether_cpu_get(i);
            if(d && add(AETHER_CAP_CPU,i,"cpu","linux-sysfs")!=AETHER_OK && first_error==AETHER_OK) first_error=AETHER_ERR_LIMIT;
        }
    }

    if(aether_gpu_scan()==AETHER_OK){
        for(size_t i=0,n=aether_gpu_count();i<n;i++){
            const aether_gpu_t *d=aether_gpu_get(i);
            if(d && add(AETHER_CAP_GPU,i,d->name,"drm")!=AETHER_OK && first_error==AETHER_OK) first_error=AETHER_ERR_LIMIT;
        }
    }

    if(aether_display_scan()==AETHER_OK){
        for(size_t i=0,n=aether_display_count();i<n;i++){
            const aether_display_t *d=aether_display_get(i);
            if(d && d->connected && add(AETHER_CAP_DISPLAY,i,d->connector,"drm")!=AETHER_OK && first_error==AETHER_OK) first_error=AETHER_ERR_LIMIT;
        }
    }

    if(aether_audio_scan()==AETHER_OK){
        for(size_t i=0,n=aether_audio_count();i<n;i++){
            const aether_audio_device_t *d=aether_audio_get(i);
            if(d && add(AETHER_CAP_AUDIO,i,d->name,"alsa")!=AETHER_OK && first_error==AETHER_OK) first_error=AETHER_ERR_LIMIT;
        }
    }

    if(aether_camera_scan()==AETHER_OK){
        for(size_t i=0,n=aether_camera_count();i<n;i++){
            const aether_camera_t *d=aether_camera_get(i);
            if(d && add(AETHER_CAP_CAMERA,i,d->name,"v4l2")!=AETHER_OK && first_error==AETHER_OK) first_error=AETHER_ERR_LIMIT;
        }
    }

    if(aether_bluetooth_scan()==AETHER_OK){
        for(size_t i=0,n=aether_bluetooth_count();i<n;i++){
            const aether_bluetooth_adapter_t *d=aether_bluetooth_get(i);
            if(d && add(AETHER_CAP_BLUETOOTH,i,d->name,"sysfs")!=AETHER_OK && first_error==AETHER_OK) first_error=AETHER_ERR_LIMIT;
        }
    }

    if(aether_network_scan()==AETHER_OK){
        for(size_t i=0,n=aether_network_count();i<n;i++){
            const aether_network_interface_t *d=aether_network_get(i);
            if(d && d->wireless && add(AETHER_CAP_WIFI,i,d->name,"sysfs")!=AETHER_OK && first_error==AETHER_OK)
                first_error=AETHER_ERR_LIMIT;
        }
    }

    if(aether_tpm_refresh()==AETHER_OK){
        for(size_t i=0,n=aether_tpm_count();i<n;i++){
            aether_tpm_device_t d;
            if(aether_tpm_get(i,&d)==AETHER_OK){
                if(add(AETHER_CAP_TPM,i,d.device_path,"tpm")!=AETHER_OK && first_error==AETHER_OK)
                    first_error=AETHER_ERR_LIMIT;
            }
        }
    }

    if(aether_battery_scan()==AETHER_OK){
        for(size_t i=0,n=aether_battery_count();i<n;i++){
            const aether_battery_t *d=aether_battery_get(i);
            if(d && add(AETHER_CAP_BATTERY,i,d->name,"power-supply")!=AETHER_OK && first_error==AETHER_OK) first_error=AETHER_ERR_LIMIT;
        }
    }

    return first_error;
}

const aether_capability_record_t *aether_capability_find_kind(aether_capability_kind_t kind){
    for(size_t i=0;i<aether_capability_count();i++){
        const aether_capability_record_t *r=aether_capability_get(i);
        if(r && r->kind==kind && r->available) return r;
    }
    return NULL;
}
