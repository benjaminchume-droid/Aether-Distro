#define _GNU_SOURCE
#include "aether/crypto.h"
#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/random.h>
#include <unistd.h>

aether_status_t aether_crypto_random(void *buffer,size_t size){
    if(size && !buffer) return AETHER_ERR_INVALID;
    unsigned char *p=(unsigned char*)buffer;
    size_t remaining=size;
    while(remaining){
        ssize_t n=getrandom(p,remaining,0);
        if(n>0){p+=(size_t)n;remaining-=(size_t)n;continue;}
        if(n<0 && (errno==EINTR || errno==EAGAIN)) continue;
        return AETHER_ERR_IO;
    }
    return AETHER_OK;
}

int aether_crypto_equal(const void *a,const void *b,size_t size){
    if(size && (!a || !b)) return 0;
    const unsigned char *pa=(const unsigned char*)a;
    const unsigned char *pb=(const unsigned char*)b;
    unsigned char diff=0;
    for(size_t i=0;i<size;i++) diff|=(unsigned char)(pa[i]^pb[i]);
    return diff==0;
}

void aether_crypto_wipe(void *buffer,size_t size){
    volatile unsigned char *p=(volatile unsigned char*)buffer;
    if(!p) return;
    while(size--) *p++=0;
}
