#include "../include/aether/battery.h"
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#define MAX_BAT 16
static aether_battery_t a[MAX_BAT];static size_t n;
aether_status_t aether_battery_scan(void){n=0;DIR*d=opendir("/sys/class/power_supply");if(!d)return AETHER_ERR_IO;struct dirent*e;while((e=readdir(d))&&n<MAX_BAT){if(strncmp(e->d_name,"BAT",3))continue;snprintf(a[n].name,sizeof a[n].name,"%s",e->d_name);char p[256];snprintf(p,sizeof p,"/sys/class/power_supply/%s/capacity",e->d_name);FILE*f=fopen(p,"r");a[n].capacity=-1;if(f){fscanf(f,"%d",&a[n].capacity);fclose(f);}snprintf(p,sizeof p,"/sys/class/power_supply/%s/status",e->d_name);f=fopen(p,"r");a[n].status=0;if(f){char s[32];if(fgets(s,sizeof s,f)){a[n].status=!strncmp(s,"Charging",8)?1:!strncmp(s,"Discharging",11)?2:!strncmp(s,"Full",4)?3:0;}fclose(f);}++n;}closedir(d);return AETHER_OK;}
size_t aether_battery_count(void){return n;}const aether_battery_t*aether_battery_get(size_t i){return i<n?&a[i]:NULL;}
