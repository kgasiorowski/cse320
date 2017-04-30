#include "debug.h"
#include "arraylist.h"
#include "foreach.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>

static pthread_key_t key;
static pthread_once_t key_once = PTHREAD_ONCE_INIT;

static void make_key(){

    pthread_key_create(&key, NULL);

}

pthread_key_t getkey(){

    return key;

}

void *foreach_init(arraylist_t *self){

    if(self == NULL){

        errno = EINVAL;
        return NULL;

    }

    if(self->length == 0 || self->item_size == 0 || self->base == NULL){

        return NULL;

    }

    foreach_t *foreach_data;
    pthread_once(&key_once, make_key);

    foreach_data = (foreach_t*)malloc(sizeof(foreach_t));
    foreach_data->current_index = 0;
    foreach_data->list = self;

    pthread_setspecific(key, foreach_data);

    //Read lock here
    debug("%s","Waiting for lock\n");
    sem_wait(&self->lock);
    debug("%s","Locked lock\n");

    self->readcnt++;
    debug("Reader count: %d\n", self->readcnt);

    if(self->readcnt == 1){
        //First in
        debug("%s","Waiting for write_lock\n");
        sem_wait(&self->write_lock);
        debug("%s","Locked write_lock\n");

    }

    debug("%s","Unlocking lock\n");
    sem_post(&self->lock);
    debug("%s","Unlocked lock\n");

    return get_index_al(self, 0);

}

void *foreach_next(arraylist_t *self, void *data){

    size_t current_index;

    current_index = get_data_al(self, data);

    if(current_index == self->length-1){

        foreach_break_f();
        return NULL;

    }else{

        foreach_t *foreach_data = pthread_getspecific(key);

        foreach_data->current_index++;
        return get_index_al(self, current_index+1);

    }

}

size_t foreach_index(){

    foreach_t *foreach_data = pthread_getspecific(key);

    if(foreach_data == NULL)
        return UINT_MAX;
    else
        return foreach_data->current_index;

}

bool foreach_break_f(){

    //Read unlock here

    foreach_t *foreach_data = pthread_getspecific(key);

    arraylist_t *self = foreach_data->list;

    sem_wait(&self->lock);

    self->readcnt--;
    if(self->readcnt == 0) //Last out
        sem_post(&self->write_lock);

    sem_post(&self->lock);

    free(foreach_data);
    return true;

}

int32_t apply(arraylist_t *self, int32_t (*application)(void*)){

    //Application refers to the function that changes each item in the list

    int32_t ret = 1;

    const size_t itemsize =   self->item_size;
    const size_t listlength = self->length;
    // const size_t capacity =   self->capacity;
    const void *base =        self->base;

    //Create a copy of the arraylist as a backup
    void *base_backup = calloc(itemsize, listlength);
    memcpy(base_backup, (void*)base, itemsize*listlength);

    //Now iterate through the arraylist
    int i;
    for(i = 0; i < listlength; i++){

        size_t offset = (i*itemsize);
        void *current_item = (char*)base + offset;

        int32_t rt = application(current_item);

        //If the thing didn't return as zero, restore the item that was changed
        if(rt == -1){

            debug("%s\n","Application failed!");

            void *backup_item = (char*)base_backup + offset;
            memcpy(current_item, backup_item, itemsize);
            ret = 0;

        }

    }

    //Release our backup
    free(base_backup);

    return ret;

}
