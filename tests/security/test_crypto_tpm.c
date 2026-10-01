#include <assert.h>
#include <string.h>
#include "aether/crypto.h"
#include "aether/tpm_service.h"

int main(void){
    unsigned char a[32]={0};
    unsigned char b[32]={0};
    assert(aether_crypto_random(a,sizeof(a))==AETHER_OK);
    memcpy(b,a,sizeof(a));
    assert(aether_crypto_equal(a,b,sizeof(a))==1);
    b[0]^=1u;
    assert(aether_crypto_equal(a,b,sizeof(a))==0);
    aether_crypto_wipe(b,sizeof(b));
    for(size_t i=0;i<sizeof(b);i++) assert(b[i]==0);

    aether_status_t st=aether_tpm_refresh();
    assert(st==AETHER_OK || st==AETHER_ERR_UNAVAILABLE || st==AETHER_ERR_IO);
    if(st==AETHER_OK){
        assert(aether_tpm_available()==1);
        assert(aether_tpm_count()>0);
        aether_tpm_device_t device;
        assert(aether_tpm_get(0,&device)==AETHER_OK);
        assert(device.available==1);
    }else{
        assert(aether_tpm_available()==0);
        assert(aether_tpm_count()==0);
    }
    return 0;
}
