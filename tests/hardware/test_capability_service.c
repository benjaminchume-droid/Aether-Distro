#include <assert.h>
#include "aether/capability_service.h"

int main(void){
    assert(aether_capability_service_init()==AETHER_OK);
    aether_status_t st=aether_capability_refresh();
    assert(st==AETHER_OK || st==AETHER_ERR_LIMIT);

    size_t total=aether_capability_count();
    assert(total>0);

    const aether_capability_record_t *cpu=aether_capability_find_kind(AETHER_CAP_CPU);
    assert(cpu!=0);
    assert(cpu->available==1);
    assert(cpu->name[0]!='\0');
    assert(cpu->provider[0]!='\0');

    for(size_t i=0;i<total;i++){
        const aether_capability_record_t *entry=aether_capability_get(i);
        assert(entry!=0);
        assert(entry->available!=0);
        assert(entry->name!=0 && entry->name[0]!='\0');
        assert(entry->provider!=0 && entry->provider[0]!='\0');
    }
    return 0;
}
