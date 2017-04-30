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

    size_t itemsize = self->item_size;
    // size_t listlength = self->length;
    size_t capacity = self->capacity;

    //Create a copy of the arraylist as a backup
    arraylist_t *list_copy = calloc(itemsize, capacity);
    memcpy(self, list_copy, itemsize*capacity);



    //Release our backup
    free(list_copy);

    return 0;

}
