#include <aether/thermal.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#define MAX_THERMAL 128
static aether_thermal_zone_t a[MAX_THERMAL];static size_t n;
aether_status_t aether_thermal_scan(void){n=0;DIR*d=opendir("/sys/class/thermal");if(!d)return AETHER_ERR_IO;struct dirent*e;while((e=readdir(d))&&n<MAX_THERMAL){if(strncmp(e->d_name,"thermal_zone",12))continue;char p[256];snprintf(p,sizeof p,"/sys/class/thermal/%s/type",e->d_name);FILE*f=fopen(p,"r");if(!f)continue;fgets(a[n].name,sizeof a[n].name,f);fclose(f);a[n].name[strcspn(a[n].name,"\n")]=0;snprintf(p,sizeof p,"/sys/class/thermal/%s/temp",e->d_name);f=fopen(p,"r");a[n].temperature_millidegrees=0;if(f){fscanf(f,"%d",&a[n].temperature_millidegrees);fclose(f);}++n;}closedir(d);return AETHER_OK;}
size_t aether_thermal_count(void){return n;}const aether_thermal_zone_t*aether_thermal_get(size_t i){return i<n?&a[i]:NULL;}
