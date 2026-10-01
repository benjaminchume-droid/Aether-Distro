#define _GNU_SOURCE
#include "../include/aether/pam_provider.h"
#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "aether/account_service.h"
#include "aether/authentication_service.h"
#include "aether/crypto.h"

typedef struct pam_handle pam_handle_t;
struct pam_message { int msg_style; const char *msg; };
struct pam_response { char *resp; int resp_retcode; };
struct pam_conv { int (*conv)(int,const struct pam_message **,struct pam_response **,void *); void *data; };

typedef int (*pam_start_fn)(const char*,const char*,const struct pam_conv*,pam_handle_t**);
typedef int (*pam_end_fn)(pam_handle_t*,int);
typedef int (*pam_authenticate_fn)(pam_handle_t*,unsigned);
typedef int (*pam_acct_mgmt_fn)(pam_handle_t*,unsigned);

#define PAM_SUCCESS 0
#define PAM_OPEN_ERR 1
#define PAM_SYMBOL_ERR 2
#define PAM_SERVICE_ERR 3
#define PAM_SYSTEM_ERR 4
#define PAM_BUF_ERR 5
#define PAM_CONV_ERR 6
#define PAM_PERM_DENIED 7
#define PAM_AUTH_ERR 9
#define PAM_USER_UNKNOWN 10
#define PAM_CRED_INSUFFICIENT 8
#define PAM_AUTHINFO_UNAVAIL 9
#define PAM_USER_ERR 13
#define PAM_MAXTRIES 11
#define PAM_ACCT_EXPIRED 13
#define PAM_NEW_AUTHTOK_REQD 12
#define PAM_CRED_ERR 19
#define PAM_CRED_EXPIRED 21
#define PAM_AUTHTOK_ERR 20
#define PAM_AUTHTOK_RECOVER_ERR 21
#define PAM_AUTHTOK_LOCK_BUSY 22
#define PAM_CONV_ECHO_ON 2
#define PAM_CONV_ECHO_OFF 1
#define PAM_CONV_ERROR_MSG 3
#define PAM_CONV_TEXT_INFO 4

typedef struct {
    void *library;
    pam_start_fn pam_start;
    pam_end_fn pam_end;
    pam_authenticate_fn pam_authenticate;
    pam_acct_mgmt_fn pam_acct_mgmt;
    char service[128];
    int available;
} pam_runtime_t;

typedef struct {
    const char *username;
    char *password;
    size_t password_size;
} pam_conversation_t;

static pam_runtime_t runtime;

static int pam_conversation(int count,const struct pam_message **messages,struct pam_response **responses,void *data){
    if(count<=0 || !messages || !responses || !data) return PAM_CONV_ERR;
    pam_conversation_t *ctx=(pam_conversation_t*)data;
    struct pam_response *out=calloc((size_t)count,sizeof(*out));
    if(!out) return PAM_CONV_ERR;

    for(int i=0;i<count;i++){
        const struct pam_message *message=messages[i];
        if(!message){free(out);return PAM_CONV_ERR;}
        if(message->msg_style==PAM_CONV_ECHO_OFF){
            if(!ctx->password || !ctx->password_size) {free(out);return PAM_CONV_ERR;}
            out[i].resp=malloc(ctx->password_size+1);
            if(!out[i].resp){free(out);return PAM_CONV_ERR;}
            memcpy(out[i].resp,ctx->password,ctx->password_size);
            out[i].resp[ctx->password_size]='\0';
        }else if(message->msg_style==PAM_CONV_ECHO_ON){
            if(ctx->username){
                size_t len=strlen(ctx->username);
                out[i].resp=malloc(len+1);
                if(!out[i].resp){free(out);return PAM_CONV_ERR;}
                memcpy(out[i].resp,ctx->username,len+1);
            }
        }else if(message->msg_style==PAM_CONV_TEXT_INFO || message->msg_style==PAM_CONV_ERROR_MSG){
            out[i].resp=NULL;
        }else{
            free(out);
            return PAM_CONV_ERR;
        }
    }
    *responses=out;
    return PAM_SUCCESS;
}

static void free_conversation_responses(struct pam_response *responses,int count){
    if(!responses) return;
    for(int i=0;i<count;i++){
        if(responses[i].resp){
            size_t len=strlen(responses[i].resp);
            aether_crypto_wipe(responses[i].resp,len);
            free(responses[i].resp);
        }
    }
    free(responses);
}

static aether_status_t pam_verify(aether_id_t user_id,uint64_t challenge_id,const void *credential,size_t credential_size,void *context){
    (void)challenge_id;
    pam_runtime_t *rt=(pam_runtime_t*)context;
    if(!rt || !rt->available || !credential || credential_size==0) return AETHER_ERR_INVALID;
    if(memchr(credential,'\0',credential_size)!=NULL) return AETHER_ERR_INVALID;

    aether_account_t account;
    if(aether_account_lookup_uid((uint32_t)user_id,&account)!=AETHER_OK) return AETHER_ERR_NOT_FOUND;

    char *password=malloc(credential_size+1);
    if(!password) return AETHER_ERR_NOMEM;
    memcpy(password,credential,credential_size);
    password[credential_size]='\0';

    pam_conversation_t conversation={account.name,password,credential_size};
    struct pam_conv conv={pam_conversation,&conversation};
    pam_handle_t *handle=NULL;
    int rc=rt->pam_start(rt->service,account.name,&conv,&handle);
    if(rc!=PAM_SUCCESS){aether_crypto_wipe(password,credential_size);free(password);return AETHER_ERR_UNAVAILABLE;}

    rc=rt->pam_authenticate(handle,0);
    int acct_rc=rc==PAM_SUCCESS ? rt->pam_acct_mgmt(handle,0) : rc;
    rt->pam_end(handle,acct_rc);

    aether_crypto_wipe(password,credential_size);
    free(password);

    if(rc==PAM_SUCCESS && acct_rc==PAM_SUCCESS) return AETHER_OK;
    if(acct_rc==PAM_AUTH_ERR || acct_rc==PAM_USER_UNKNOWN || acct_rc==PAM_CRED_INSUFFICIENT ||
       acct_rc==PAM_MAXTRIES || acct_rc==PAM_PERM_DENIED || acct_rc==PAM_ACCT_EXPIRED)
        return AETHER_ERR_PERMISSION;
    return AETHER_ERR_UNAVAILABLE;
}

aether_status_t aether_pam_password_provider_register(const char *service_name){
    if(!service_name || !*service_name || strlen(service_name)>=sizeof(runtime.service))
        return AETHER_ERR_INVALID;
    if(runtime.available) return AETHER_ERR_EXISTS;

    runtime.library=dlopen("libpam.so.0",RTLD_NOW|RTLD_LOCAL);
    if(!runtime.library) return AETHER_ERR_UNAVAILABLE;

    runtime.pam_start=(pam_start_fn)dlsym(runtime.library,"pam_start");
    runtime.pam_end=(pam_end_fn)dlsym(runtime.library,"pam_end");
    runtime.pam_authenticate=(pam_authenticate_fn)dlsym(runtime.library,"pam_authenticate");
    runtime.pam_acct_mgmt=(pam_acct_mgmt_fn)dlsym(runtime.library,"pam_acct_mgmt");
    if(!runtime.pam_start || !runtime.pam_end || !runtime.pam_authenticate || !runtime.pam_acct_mgmt){
        dlclose(runtime.library);
        memset(&runtime,0,sizeof(runtime));
        return AETHER_ERR_UNAVAILABLE;
    }

    snprintf(runtime.service,sizeof(runtime.service),"%s",service_name);
    runtime.available=1;

    aether_auth_provider_t provider={
        .method=AETHER_AUTH_PASSWORD,
        .flags=0,
        .provider="linux-pam",
        .begin=NULL,
        .verify=pam_verify,
        .cancel=NULL,
        .context=&runtime
    };
    aether_status_t st=aether_authentication_register(&provider);
    if(st!=AETHER_OK){
        dlclose(runtime.library);
        memset(&runtime,0,sizeof(runtime));
        return st;
    }
    return AETHER_OK;
}

void aether_pam_password_provider_shutdown(void){
    if(runtime.library) dlclose(runtime.library);
    memset(&runtime,0,sizeof(runtime));
}

int aether_pam_password_provider_available(void){return runtime.available;}
