#include "../include/aether/permission_subject.h"
#include <string.h>

#define MAX_PERMISSION_SUBJECTS 512

static aether_permission_subject_t subjects[MAX_PERMISSION_SUBJECTS];
static size_t count;

aether_status_t aether_permission_subject_init(void){
    count=0;
    memset(subjects,0,sizeof(subjects));
    return AETHER_OK;
}

aether_status_t aether_permission_subject_register(const aether_permission_subject_t *subject){
    if(!subject || !subject->subject_id || !subject->kind || !subject->name[0])
        return AETHER_ERR_INVALID;
    if(subject->kind<AETHER_SUBJECT_USER || subject->kind>AETHER_SUBJECT_WORKLOAD)
        return AETHER_ERR_INVALID;
    if(count>=MAX_PERMISSION_SUBJECTS) return AETHER_ERR_LIMIT;
    if(aether_permission_subject_find(subject->subject_id)) return AETHER_ERR_EXISTS;
    subjects[count++]=*subject;
    return AETHER_OK;
}

aether_status_t aether_permission_subject_unregister(aether_id_t subject_id){
    if(!subject_id) return AETHER_ERR_INVALID;
    for(size_t i=0;i<count;i++){
        if(subjects[i].subject_id==subject_id){
            subjects[i]=subjects[--count];
            memset(&subjects[count],0,sizeof(subjects[count]));
            return AETHER_OK;
        }
    }
    return AETHER_ERR_NOT_FOUND;
}

const aether_permission_subject_t *aether_permission_subject_find(aether_id_t subject_id){
    if(!subject_id) return NULL;
    for(size_t i=0;i<count;i++)
        if(subjects[i].subject_id==subject_id) return &subjects[i];
    return NULL;
}

size_t aether_permission_subject_count(void){return count;}

aether_status_t aether_permission_subject_get(size_t index,aether_permission_subject_t *out){
    if(!out) return AETHER_ERR_INVALID;
    if(index>=count) return AETHER_ERR_NOT_FOUND;
    *out=subjects[index];
    return AETHER_OK;
}

void aether_permission_subject_shutdown(void){
    count=0;
    memset(subjects,0,sizeof(subjects));
}
