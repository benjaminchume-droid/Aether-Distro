#include <assert.h>
#include "aether/biometric_service.h"

int main(void){
    assert(aether_biometric_init()==AETHER_OK);
    aether_biometric_provider_t provider={7,AETHER_BIOMETRIC_FINGERPRINT,"test-provider","external-provider",1,1};
    assert(aether_biometric_register(&provider)==AETHER_OK);
    assert(aether_biometric_count()==1);
    assert(aether_biometric_find(AETHER_BIOMETRIC_FINGERPRINT)!=0);
    assert(aether_biometric_find(AETHER_BIOMETRIC_FACE)==0);
    assert(aether_biometric_unregister(7)==AETHER_OK);
    assert(aether_biometric_count()==0);
    aether_biometric_shutdown();
    return 0;
}
