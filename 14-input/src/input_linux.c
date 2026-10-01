#include "../include/aether/input.h"
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#define MAX_INPUT 128
static aether_input_device_t inputs[MAX_INPUT];static size_t count;
static aether_input_kind_t classify(const char*n){if(strstr(n,"keyboard"))return AETHER_INPUT_KEYBOARD;if(strstr(n,"mouse")||strstr(n,"touchpad"))return AETHER_INPUT_POINTER;if(strstr(n,"touchscreen"))return AETHER_INPUT_TOUCH;if(strstr(n,"gamepad")||strstr(n,"joystick"))return AETHER_INPUT_GAMEPAD;return AETHER_INPUT_OTHER;}
aether_status_t aether_input_scan(void){count=0;DIR*d=opendir("/sys/class/input");if(!d)return AETHER_ERR_IO;struct dirent*e;while((e=readdir(d))&&count<MAX_INPUT){if(strncmp(e->d_name,"event",5))continue;aether_input_device_t*x=&inputs[count];x->id=(unsigned)count;char p[256];snprintf(p,sizeof p,"/sys/class/input/%s/device/name",e->d_name);FILE*f=fopen(p,"r");x->name[0]=0;if(f){fgets(x->name,sizeof x->name,f);fclose(f);}x->name[strcspn(x->name,"\n")]=0;snprintf(x->path,sizeof x->path,"/dev/input/%s",e->d_name);x->kind=classify(x->name);++count;}closedir(d);return AETHER_OK;}
size_t aether_input_count(void){return count;}const aether_input_device_t*aether_input_get(size_t i){return i<count?&inputs[i]:NULL;}
