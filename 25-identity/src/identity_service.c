#define _POSIX_C_SOURCE 200809L
#include "aether/identity_service.h"
#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static aether_status_t fill_record(const struct passwd *pw,aether_identity_record_t *out){
 if(!pw||!out||!pw->pw_name||!pw->pw_dir)return AETHER_ERR_INVALID;
 memset(out,0,sizeof(*out));
 out->uid=(uint32_t)pw->pw_uid; out->gid=(uint32_t)pw->pw_gid; out->user_id=(aether_id_t)pw->pw_uid;
 snprintf(out->name,sizeof(out->name),"%s",pw->pw_name);
 snprintf(out->home,sizeof(out->home),"%s",pw->pw_dir);
 if(pw->pw_shell)snprintf(out->shell,sizeof(out->shell),"%s",pw->pw_shell);
 return AETHER_OK;
}

aether_status_t aether_identity_current(aether_identity_record_t *out){return aether_identity_lookup((uint32_t)getuid(),out);}

aether_status_t aether_identity_lookup(uint32_t uid,aether_identity_record_t *out){
 if(!out)return AETHER_ERR_INVALID;
 struct passwd pw,*result=0;
 long n=sysconf(_SC_GETPW_R_SIZE_MAX); if(n<1024)n=16384;
 char *buf=(char*)malloc((size_t)n); if(!buf)return AETHER_ERR_NOMEM;
 int rc=getpwuid_r((uid_t)uid,&pw,buf,(size_t)n,&result);
 aether_status_t status=rc?AETHER_ERR_IO:(result?fill_record(result,out):AETHER_ERR_NOT_FOUND);
 free(buf); return status;
}
