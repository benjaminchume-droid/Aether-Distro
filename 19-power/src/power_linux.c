#include "../include/aether/power.h"
#include <dirent.h>
#include <stdio.h>
#include <string.h>
static unsigned read_flag(const char*p){FILE*f=fopen(p,"r");unsigned v=0;if(f){char s[32];if(fgets(s,sizeof s,f))v=!strncmp(s,"1",1)||!strncmp(s,"yes",3);fclose(f);}return v;}
aether_status_t aether_power_state_read(aether_power_state_t*s){if(!s)return AETHER_ERR_INVALID;s->ac_online=0;s->suspended=0;DIR*d=opendir("/sys/class/power_supply");if(!d)return AETHER_ERR_UNAVAILABLE;struct dirent*e;while((e=readdir(d))){if(e->d_name[0]=='.')continue;char p[256],t[32];snprintf(p,sizeof p,"/sys/class/power_supply/%s/type",e->d_name);FILE*f=fopen(p,"r");t[0]=0;if(f){fgets(t,sizeof t,f);fclose(f);}if(!strncmp(t,"Mains",5)||!strncmp(t,"USB",3)){snprintf(p,sizeof p,"/sys/class/power_supply/%s/online",e->d_name);if(read_flag(p))s->ac_online=1;}}closedir(d);return AETHER_OK;}
