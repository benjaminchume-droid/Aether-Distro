#include "../include/aether/firmware.h"
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#define MAX_FW 256
static aether_firmware_entry_t a[MAX_FW];static size_t n;
static void walk(const char*base){if(n>=MAX_FW)return;DIR*d=opendir(base);if(!d)return;struct dirent*e;while((e=readdir(d))&&n<MAX_FW){if(e->d_name[0]=='.')continue;char p[512];snprintf(p,sizeof p,"%s/%s",base,e->d_name);DIR*sub=opendir(p);if(sub){closedir(sub);continue;}snprintf(a[n].name,sizeof a[n].name,"%s",e->d_name);snprintf(a[n].path,sizeof a[n].path,"%s",p);++n;}closedir(d);}
aether_status_t aether_firmware_scan(void){n=0;walk("/lib/firmware");return AETHER_OK;}
size_t aether_firmware_count(void){return n;}const aether_firmware_entry_t*aether_firmware_get(size_t i){return i<n?&a[i]:NULL;}
