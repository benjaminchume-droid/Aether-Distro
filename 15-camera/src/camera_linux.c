#include <aether/camera.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#define MAX_CAM 32
static aether_camera_t a[MAX_CAM];static size_t n;
aether_status_t aether_camera_scan(void){n=0;DIR*d=opendir("/sys/class/video4linux");if(!d)return AETHER_ERR_IO;struct dirent*e;while((e=readdir(d))&&n<MAX_CAM){if(strncmp(e->d_name,"video",5))continue;char p[256];snprintf(p,sizeof p,"/sys/class/video4linux/%s/name",e->d_name);FILE*f=fopen(p,"r");if(!f)continue;char name[128];if(!fgets(name,sizeof name,f)){fclose(f);continue;}fclose(f);a[n].id=(unsigned)n;snprintf(a[n].path,sizeof a[n].path,"/dev/%s",e->d_name);snprintf(a[n].name,sizeof a[n].name,"%s",name);a[n].name[strcspn(a[n].name,"\n")]=0;++n;}closedir(d);return AETHER_OK;}
size_t aether_camera_count(void){return n;}const aether_camera_t*aether_camera_get(size_t i){return i<n?&a[i]:NULL;}
