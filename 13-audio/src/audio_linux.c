#include "../include/aether/audio.h"
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#define MAX_AUDIO 64
static aether_audio_device_t a[MAX_AUDIO];static size_t n;
aether_status_t aether_audio_scan(void){n=0;DIR*d=opendir("/sys/class/sound");if(!d)return AETHER_ERR_IO;struct dirent*e;while((e=readdir(d))&&n<MAX_AUDIO){if(strncmp(e->d_name,"card",4))continue;snprintf(a[n].id,sizeof a[n].id,"%s",e->d_name);char p[256];snprintf(p,sizeof p,"/sys/class/sound/%s/id",e->d_name);FILE*f=fopen(p,"r");a[n].name[0]=0;if(f){fgets(a[n].name,sizeof a[n].name,f);fclose(f);}a[n].name[strcspn(a[n].name,"\n")]=0;snprintf(a[n].type,sizeof a[n].type,"alsa");++n;}closedir(d);return AETHER_OK;}
size_t aether_audio_count(void){return n;}const aether_audio_device_t*aether_audio_get(size_t i){return i<n?&a[i]:NULL;}
