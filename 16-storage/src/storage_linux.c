#include "../include/aether/storage.h"
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#define MAX_BLOCK 128
static aether_block_device_t a[MAX_BLOCK];static size_t n;
static uint64_t read_u64(const char*p){FILE*f=fopen(p,"r");unsigned long long v=0;if(f){fscanf(f,"%llu",&v);fclose(f);}return (uint64_t)v;}
aether_status_t aether_storage_scan(void){n=0;DIR*d=opendir("/sys/class/block");if(!d)return AETHER_ERR_IO;struct dirent*e;while((e=readdir(d))&&n<MAX_BLOCK){if(!strcmp(e->d_name,".")||!strcmp(e->d_name,".."))continue;if(strstr(e->d_name,"loop")||strstr(e->d_name,"ram")||strstr(e->d_name,"sr"))continue;snprintf(a[n].name,sizeof a[n].name,"%s",e->d_name);snprintf(a[n].devnode,sizeof a[n].devnode,"/dev/%s",e->d_name);char p[256];snprintf(p,sizeof p,"/sys/class/block/%s/size",e->d_name);a[n].size_bytes=read_u64(p)*512ULL;snprintf(p,sizeof p,"/sys/class/block/%s/removable",e->d_name);a[n].removable=(unsigned)read_u64(p);++n;}closedir(d);return AETHER_OK;}
size_t aether_storage_count(void){return n;}const aether_block_device_t*aether_storage_get(size_t i){return i<n?&a[i]:NULL;}
