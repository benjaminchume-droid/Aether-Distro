#include <aether/bluetooth.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#define MAX_BT 32
static aether_bluetooth_adapter_t a[MAX_BT];static size_t n;
aether_status_t aether_bluetooth_scan(void){n=0;DIR*d=opendir("/sys/class/bluetooth");if(!d)return AETHER_ERR_UNAVAILABLE;struct dirent*e;while((e=readdir(d))&&n<MAX_BT){if(e->d_name[0]=='.')continue;snprintf(a[n].name,sizeof a[n].name,"%s",e->d_name);char p[256];snprintf(p,sizeof p,"/sys/class/bluetooth/%s/address",e->d_name);FILE*f=fopen(p,"r");a[n].address[0]=0;if(f){fgets(a[n].address,sizeof a[n].address,f);fclose(f);}a[n].address[strcspn(a[n].address,"\n")]=0;++n;}closedir(d);return AETHER_OK;}
size_t aether_bluetooth_count(void){return n;}const aether_bluetooth_adapter_t*aether_bluetooth_get(size_t i){return i<n?&a[i]:NULL;}
