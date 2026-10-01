#include <aether/device_manager.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#define AETHER_MAX_DEVICES 256
static aether_device_t devices[AETHER_MAX_DEVICES];
static char names[AETHER_MAX_DEVICES][128], vendors[AETHER_MAX_DEVICES][128], classes[AETHER_MAX_DEVICES][128];
static size_t device_count_value;
static int read_line(const char *path,char *out,size_t n){FILE*f=fopen(path,"r");if(!f)return -1;if(!fgets(out,(int)n,f)){fclose(f);return -1;}fclose(f);out[strcspn(out,"\n")]=0;return 0;}
static void add_device(const char*n,const char*v,const char*c){if(device_count_value>=AETHER_MAX_DEVICES)return;aether_device_t*d=&devices[device_count_value];d->id=(aether_id_t)(device_count_value+1);snprintf(names[device_count_value],128,"%s",n);snprintf(vendors[device_count_value],128,"%s",v?v:"");snprintf(classes[device_count_value],128,"%s",c?c:"");d->name=names[device_count_value];d->vendor=vendors[device_count_value];d->class_name=classes[device_count_value];d->capability_count=0;d->capabilities=NULL;++device_count_value;}
aether_status_t aether_device_manager_init(void){device_count_value=0;return AETHER_OK;}
aether_status_t aether_device_scan(void){device_count_value=0;DIR*d=opendir("/sys/class");if(!d)return AETHER_ERR_IO;struct dirent*e;while((e=readdir(d))&&device_count_value<AETHER_MAX_DEVICES){if(e->d_name[0]=='.')continue;char base[256],v[128],c[128],uevent[320];snprintf(base,sizeof base,"/sys/class/%s",e->d_name);struct stat st;if(stat(base,&st)!=0)continue;v[0]=c[0]=0;snprintf(uevent,sizeof uevent,"%s/uevent",base);read_line(uevent,v,sizeof v);char sub[320];snprintf(sub,sizeof sub,"%s/subsystem",base);char link[256];ssize_t len=readlink(sub,link,sizeof link-1);if(len>0){link[len]=0;const char*p=strrchr(link,'/');snprintf(c,sizeof c,"%s",p?p+1:link);}add_device(e->d_name,v,c);}closedir(d);return AETHER_OK;}
size_t aether_device_count(void){return device_count_value;}
const aether_device_t*aether_device_get(size_t i){return i<device_count_value?&devices[i]:NULL;}
