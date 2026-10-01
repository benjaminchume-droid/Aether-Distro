#include "aether/storage_service.h"
#include <sys/statvfs.h>
#include <stdio.h>
#include <string.h>
#define MAX_VOLUMES 128
static aether_volume_t volumes[MAX_VOLUMES]; static size_t count;
static void add_mount(const char *path){
 if(count>=MAX_VOLUMES)return;
 struct statvfs st; if(statvfs(path,&st)!=0)return;
 aether_volume_t *v=&volumes[count++]; snprintf(v->name,sizeof(v->name),"volume-%zu",count);
 snprintf(v->mountpoint,sizeof(v->mountpoint),"%s",path);
 v->total_bytes=(uint64_t)st.f_blocks*st.f_frsize; v->available_bytes=(uint64_t)st.f_bavail*st.f_frsize;
 v->readonly=0;
}
aether_status_t aether_storage_service_refresh(void){count=0;add_mount("/");add_mount("/tmp");return count?AETHER_OK:AETHER_ERR_UNAVAILABLE;}
aether_status_t aether_storage_volume_count(size_t *out){if(!out)return AETHER_ERR_INVALID;*out=count;return AETHER_OK;}
aether_status_t aether_storage_volume_get(size_t i,aether_volume_t*out){if(!out)return AETHER_ERR_INVALID;if(i>=count)return AETHER_ERR_NOT_FOUND;*out=volumes[i];return AETHER_OK;}
