#include <assert.h>
#include <string.h>
#include "aether/permission_subject.h"

int main(void){
    assert(aether_permission_subject_init()==AETHER_OK);
    aether_permission_subject_t subject={
        .subject_id=42,
        .kind=AETHER_SUBJECT_APP,
        .uid=1000,
        .name="test.app",
        .executable="/opt/test.app/bin/app",
        .flags=0
    };
    assert(aether_permission_subject_register(&subject)==AETHER_OK);
    assert(aether_permission_subject_count()==1);
    const aether_permission_subject_t *found=aether_permission_subject_find(42);
    assert(found!=0);
    assert(found->kind==AETHER_SUBJECT_APP);
    assert(strcmp(found->name,"test.app")==0);
    aether_permission_subject_t copy;
    assert(aether_permission_subject_get(0,&copy)==AETHER_OK);
    assert(strcmp(copy.executable,subject.executable)==0);
    assert(aether_permission_subject_unregister(42)==AETHER_OK);
    assert(aether_permission_subject_count()==0);
    aether_permission_subject_shutdown();
    return 0;
}
