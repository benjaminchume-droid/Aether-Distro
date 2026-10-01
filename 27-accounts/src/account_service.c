#define _POSIX_C_SOURCE 200809L
#include "aether/account_service.h"
#include <pwd.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#define MAX_ACCOUNTS 512
static aether_account_t accounts[MAX_ACCOUNTS];
static size_t count;

static void fill(const struct passwd *pw, aether_account_t *out) {
    memset(out, 0, sizeof(*out));
    out->user_id=(aether_id_t)pw->pw_uid;
    out->uid=(uint32_t)pw->pw_uid;
    out->gid=(uint32_t)pw->pw_gid;
    snprintf(out->name,sizeof(out->name),"%s",pw->pw_name?pw->pw_name:"");
    snprintf(out->home,sizeof(out->home),"%s",pw->pw_dir?pw->pw_dir:"");
    snprintf(out->shell,sizeof(out->shell),"%s",pw->pw_shell?pw->pw_shell:"");
    if(!pw->pw_shell || !*pw->pw_shell || strcmp(pw->pw_shell,"/usr/sbin/nologin")==0 || strcmp(pw->pw_shell,"/sbin/nologin")==0)
        out->flags|=AETHER_ACCOUNT_NO_LOGIN;
}

aether_status_t aether_account_refresh(void) {
    count=0;
    setpwent();
    struct passwd *pw;
    while(count<MAX_ACCOUNTS && (pw=getpwent())!=NULL) fill(pw,&accounts[count++]);
    endpwent();
    return count ? AETHER_OK : AETHER_ERR_UNAVAILABLE;
}

aether_status_t aether_account_current(aether_account_t *out) {
    if(!out) return AETHER_ERR_INVALID;
    struct passwd *pw=getpwuid(getuid());
    if(!pw) return AETHER_ERR_NOT_FOUND;
    fill(pw,out);
    return AETHER_OK;
}

aether_status_t aether_account_lookup_uid(uint32_t uid,aether_account_t *out) {
    if(!out) return AETHER_ERR_INVALID;
    struct passwd *pw=getpwuid((uid_t)uid);
    if(!pw) return AETHER_ERR_NOT_FOUND;
    fill(pw,out);
    return AETHER_OK;
}

aether_status_t aether_account_lookup_name(const char *name,aether_account_t *out) {
    if(!name || !*name || !out) return AETHER_ERR_INVALID;
    struct passwd *pw=getpwnam(name);
    if(!pw) return AETHER_ERR_NOT_FOUND;
    fill(pw,out);
    return AETHER_OK;
}

size_t aether_account_count(void){ return count; }

aether_status_t aether_account_get(size_t index,aether_account_t *out){
    if(!out) return AETHER_ERR_INVALID;
    if(index>=count) return AETHER_ERR_NOT_FOUND;
    *out=accounts[index];
    return AETHER_OK;
}
