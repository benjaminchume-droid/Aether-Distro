#include <aether/init.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

static int ensure_dir(const char *path){if(mkdir(path,0755)<0&&errno!=EEXIST)return -1;return 0;}
static int mount_if_missing(const char *src,const char *target,const char *fstype,unsigned long flags){
    if(ensure_dir(target)<0)return -1;
    if(mount(src,target,fstype,flags,NULL)<0 && errno!=EBUSY)return -1;
    return 0;
}
aether_status_t aether_init_early(void){
    if(getpid()!=1)return AETHER_ERR_PERMISSION;
    umask(022);
    return AETHER_OK;
}
aether_status_t aether_init_mounts(void){
    if(mount_if_missing("proc","/proc","proc",0)<0)return AETHER_ERR_IO;
    if(mount_if_missing("sysfs","/sys","sysfs",0)<0)return AETHER_ERR_IO;
    if(mount_if_missing("devtmpfs","/dev","devtmpfs",0)<0)return AETHER_ERR_IO;
    if(mount_if_missing("run","/run","tmpfs",0)<0)return AETHER_ERR_IO;
    return AETHER_OK;
}
aether_status_t aether_init_services(void){return AETHER_OK;}
aether_status_t aether_init_shutdown(void){
    umount2("/run",MNT_DETACH); umount2("/dev",MNT_DETACH);
    umount2("/sys",MNT_DETACH); umount2("/proc",MNT_DETACH); return AETHER_OK;
}
