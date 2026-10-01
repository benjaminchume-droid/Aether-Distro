#include "../include/aether/gpu.h"
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#define MAX_GPU 32
static aether_gpu_t gpus[MAX_GPU];static size_t count;
static void link_name(const char*p,char*out,size_t n){char b[256];ssize_t l=readlink(p,b,sizeof b-1);if(l<0){out[0]=0;return;}b[l]=0;const char*q=strrchr(b,'/');snprintf(out,n,"%s",q?q+1:b);}
aether_status_t aether_gpu_scan(void){count=0;DIR*d=opendir("/sys/class/drm");if(!d)return AETHER_ERR_IO;struct dirent*e;while((e=readdir(d))&&count<MAX_GPU){if(strncmp(e->d_name,"card",4)||strchr(e->d_name,'-'))continue;aether_gpu_t*g=&gpus[count];g->id=(unsigned)count;snprintf(g->name,sizeof g->name,"%s",e->d_name);char p[256];snprintf(p,sizeof p,"/sys/class/drm/%s/device/uevent",e->d_name);FILE*f=fopen(p,"r");char line[256];while(f&&fgets(line,sizeof line,f)){if(!strncmp(line,"DRIVER=",7)){snprintf(g->driver,sizeof g->driver,"%s",line+7);g->driver[strcspn(g->driver,"\n")]=0;}if(!strncmp(line,"PCI_ID=",7)){snprintf(g->vendor,sizeof g->vendor,"%s",line+7);g->vendor[strcspn(g->vendor,"\n")]=0;}}if(f)fclose(f);snprintf(p,sizeof p,"/sys/class/drm/%s/device",e->d_name);link_name(p,g->pci_address,sizeof g->pci_address);++count;}closedir(d);return AETHER_OK;}
size_t aether_gpu_count(void){return count;} const aether_gpu_t*aether_gpu_get(size_t i){return i<count?&gpus[i]:NULL;}
