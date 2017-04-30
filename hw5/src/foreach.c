#include "debug.h"
#include "arraylist.h"
#include "foreach.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>

__thread foreach_t *foreach_data;

void *foreach_init(arraylist_t *self){

    if(self == NULL){

        errno = EINVAL;
        return NULL;

    }

    if(self->length == 0 || self->item_size == 0 || self->base == NULL){

        return NULL;

    }

    foreach_data = (foreach_t*)malloc(sizeof(foreach_t));
    foreach_data->current_index = 0;
    foreach_data->list = self;

    //Read lock here
    sem_wait(&self->lock);

    self->readcnt++;
    if(self->readcnt == 1) //First in
        sem_wait(&self->write_lock);

    sem_post(&self->lock);

    return get_index_al(self, 0);

}

void *foreach_next(arraylist_t *self, void *data){

    size_t current_index;

    current_index = get_data_al(self, data);

    if(current_index == self->length-1){

        foreach_break_f();
        return NULL;

    }else{

        foreach_data->current_index++;
        return get_index_al(self, current_index+1);

    }

}

size_t foreach_index(){

    if(foreach_data == NULL)
        return UINT_MAX;
    else
        return foreach_data->current_index;

}

bool foreach_break_f(){

    //Read unlock here
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
    const size_t capacity =   self->capacity;
    const void *base =        self->base;

    //Create a copy of the arraylist as a backup
    void *base_backup = calloc(itemsize, capacity);
    memcpy(self->base, base_backup, itemsize*capacity);

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
