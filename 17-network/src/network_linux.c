#include "../include/aether/network.h"
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#define MAX_NET 128
static aether_network_interface_t a[MAX_NET];static size_t n;
static unsigned flag(const char*p){FILE*f=fopen(p,"r");unsigned v=0;if(f){char s[32];if(fgets(s,sizeof s,f))v=!strncmp(s,"up",2);fclose(f);}return v;}
aether_status_t aether_network_scan(void){n=0;DIR*d=opendir("/sys/class/net");if(!d)return AETHER_ERR_IO;struct dirent*e;while((e=readdir(d))&&n<MAX_NET){if(!strcmp(e->d_name,".")||!strcmp(e->d_name,".."))continue;snprintf(a[n].name,sizeof a[n].name,"%s",e->d_name);char p[256];snprintf(p,sizeof p,"/sys/class/net/%s/operstate",e->d_name);a[n].up=flag(p);snprintf(p,sizeof p,"/sys/class/net/%s/wireless",e->d_name);FILE*f=fopen(p,"r");a[n].wireless=f!=NULL;if(f)fclose(f);++n;}closedir(d);return AETHER_OK;}
size_t aether_network_count(void){return n;}const aether_network_interface_t*aether_network_get(size_t i){return i<n?&a[i]:NULL;}
