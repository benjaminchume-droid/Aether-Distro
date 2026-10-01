#define _GNU_SOURCE
#include "../include/aether/tpm_service.h"
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_TPM_DEVICES 8

static aether_tpm_device_t devices[MAX_TPM_DEVICES];
static size_t count;

static void read_optional(const char *path,char *out,size_t size){
    FILE *f=fopen(path,"r");
    if(!f){out[0]='\0';return;}
    if(!fgets(out,(int)size,f)) out[0]='\0';
    out[strcspn(out,"\n")]='\0';
    fclose(f);
}

static void add_device(const char *sys_name,unsigned rm){
    if(count>=MAX_TPM_DEVICES || !sys_name || strncmp(sys_name,"tpm",3)!=0) return;
    unsigned index=(unsigned)strtoul(sys_name+3,NULL,10);
    char path[128];
    snprintf(path,sizeof(path),"/dev/%s%u",rm?"tpmrm":"tpm",index);

    int fd=open(path,O_RDONLY|O_CLOEXEC);
    if(fd<0) return;
    close(fd);

    aether_tpm_device_t *d=&devices[count++];
    memset(d,0,sizeof(*d));
    snprintf(d->device_path,sizeof(d->device_path),"%s",path);
    d->resource_manager=rm;
    d->available=1;

    char p[320],base[256];
    snprintf(base,sizeof(base),"/sys/class/tpm/%s",sys_name);
    snprintf(p,sizeof(p),"%s/device/description",base);
    read_optional(p,d->description,sizeof(d->description));
    snprintf(p,sizeof(p),"%s/tpm_version_major",base);
    char v[32];
    read_optional(p,v,sizeof(v));
    if(v[0]) d->major_version=(uint32_t)strtoul(v,NULL,10);
}

aether_status_t aether_tpm_refresh(void){
    count=0;
    DIR *dir=opendir("/sys/class/tpm");
    if(!dir) return AETHER_ERR_IO;

    struct dirent *entry;
    while((entry=readdir(dir)) && count<MAX_TPM_DEVICES){
        if(strncmp(entry->d_name,"tpm",3)!=0) continue;
        add_device(entry->d_name,1);
        add_device(entry->d_name,0);
    }
    closedir(dir);
    return count?AETHER_OK:AETHER_ERR_UNAVAILABLE;
}

size_t aether_tpm_count(void){return count;}
aether_status_t aether_tpm_get(size_t index,aether_tpm_device_t *out){
    if(!out) return AETHER_ERR_INVALID;
    if(index>=count) return AETHER_ERR_NOT_FOUND;
    *out=devices[index];
    return AETHER_OK;
}
int aether_tpm_available(void){return count>0;}
