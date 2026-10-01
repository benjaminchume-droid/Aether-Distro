#include "aether/storage_service.h"
#include <sys/statvfs.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_VOLUMES 128

static aether_volume_t volumes[MAX_VOLUMES];
static size_t count;

static void decode_mountpoint(char *path){
    char *src=path,*dst=path;
    while(*src){
        if(src[0]=='\\' && src[1]=='0' && src[2]=='4' && src[3]=='0'){*dst++=' ';src+=4;}
        else if(src[0]=='\\' && src[1]=='0' && src[2]=='1' && src[3]=='1'){*dst++='\t';src+=4;}
        else if(src[0]=='\\' && src[1]=='1' && src[2]=='3' && src[3]=='4'){*dst++='\\';src+=4;}
        else {*dst++=*src++;}
    }
    *dst='\\0';
}

static int known_mount(const char *path){
    for(size_t i=0;i<count;i++) if(strcmp(volumes[i].mountpoint,path)==0) return 1;
    return 0;
}

static void add_mount(const char *path){
    if(count>=MAX_VOLUMES || !path || !*path || known_mount(path)) return;
    struct statvfs st;
    if(statvfs(path,&st)!=0) return;
    aether_volume_t *v=&volumes[count++];
    snprintf(v->name,sizeof(v->name),"volume-%zu",count);
    snprintf(v->mountpoint,sizeof(v->mountpoint),"%s",path);
    v->total_bytes=(uint64_t)st.f_blocks*st.f_frsize;
    v->available_bytes=(uint64_t)st.f_bavail*st.f_frsize;
    v->readonly=(st.f_flag & ST_RDONLY) ? 1u : 0u;
}

aether_status_t aether_storage_service_refresh(void){
    count=0;
    FILE *f=fopen("/proc/self/mountinfo","r");
    if(!f) return AETHER_ERR_IO;

    char line[4096];
    while(fgets(line,sizeof(line),f) && count<MAX_VOLUMES){
        char mountpoint[256]={0};
        if(sscanf(line,"%*s %*s %*s %*s %255s",mountpoint)!=1) continue;
        if(!mountpoint[0]) continue;
        decode_mountpoint(mountpoint);
        add_mount(mountpoint);
    }
    fclose(f);
    return count ? AETHER_OK : AETHER_ERR_UNAVAILABLE;
}

aether_status_t aether_storage_volume_count(size_t *out){
    if(!out) return AETHER_ERR_INVALID;
    *out=count;
    return AETHER_OK;
}

aether_status_t aether_storage_volume_get(size_t index,aether_volume_t *out){
    if(!out) return AETHER_ERR_INVALID;
    if(index>=count) return AETHER_ERR_NOT_FOUND;
    *out=volumes[index];
    return AETHER_OK;
}
