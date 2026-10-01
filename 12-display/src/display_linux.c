#include "../include/aether/display.h"
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#define MAX_DISPLAY 32
static aether_display_t displays[MAX_DISPLAY];static size_t count;
aether_status_t aether_display_scan(void){count=0;DIR*d=opendir("/sys/class/drm");if(!d)return AETHER_ERR_IO;struct dirent*e;while((e=readdir(d))&&count<MAX_DISPLAY){if(strncmp(e->d_name,"card",4)||!strchr(e->d_name,'-'))continue;aether_display_t*x=&displays[count];snprintf(x->connector,sizeof x->connector,"%s",e->d_name);char p[256],s[64];snprintf(p,sizeof p,"/sys/class/drm/%s/status",e->d_name);FILE*f=fopen(p,"r");x->connected=0;if(f){if(fgets(s,sizeof s,f))x->connected=!strncmp(s,"connected",9);fclose(f);}snprintf(p,sizeof p,"/sys/class/drm/%s/modes",e->d_name);f=fopen(p,"r");x->width=x->height=0;x->refresh_millihz=0;if(f&&fgets(s,sizeof s,f)){unsigned r=0;sscanf(s,"%ux%u@%u",&x->width,&x->height,&r);if(!r)sscanf(s,"%ux%u",&x->width,&x->height);x->refresh_millihz=r*1000;}if(f)fclose(f);++count;}closedir(d);return AETHER_OK;}
size_t aether_display_count(void){return count;}const aether_display_t*aether_display_get(size_t i){return i<count?&displays[i]:NULL;}
