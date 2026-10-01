#include "../include/aether/cpu.h"
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#define MAX_CPU 256
static aether_cpu_t cpus[MAX_CPU];static size_t count;
static unsigned read_u(const char*p,unsigned d){FILE*f=fopen(p,"r");unsigned v=d;if(f){if(fscanf(f,"%u",&v)!=1)v=d;fclose(f);}return v;}
aether_status_t aether_cpu_scan(void){count=0;DIR*d=opendir("/sys/devices/system/cpu");if(!d)return AETHER_ERR_IO;struct dirent*e;while((e=readdir(d))&&count<MAX_CPU){unsigned n;if(sscanf(e->d_name,"cpu%u",&n)!=1||strncmp(e->d_name,"cpu",3))continue;char p[256];cpus[count].index=n;snprintf(p,sizeof p,"/sys/devices/system/cpu/cpu%u/online",n);cpus[count].online=n==0?1:read_u(p,1);snprintf(p,sizeof p,"/sys/devices/system/cpu/cpu%u/topology/physical_package_id",n);cpus[count].package=read_u(p,0);snprintf(p,sizeof p,"/sys/devices/system/cpu/cpu%u/topology/core_id",n);cpus[count].core=read_u(p,n);cpus[count].thread=n;++count;}closedir(d);return AETHER_OK;}
size_t aether_cpu_count(void){return count;} const aether_cpu_t*aether_cpu_get(size_t i){return i<count?&cpus[i]:NULL;}
