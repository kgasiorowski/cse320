#include "arraylist.h"
#include "debug.h"
#include <errno.h>
#include <stdio.h>

/**
 * @visibility HIDDEN FROM USER
 * @return     true on success, false on failure
 */
static bool resize_al(arraylist_t* self){


    debug("%s","Entered resize\n");
    bool ret = false;

    if(self == NULL){

        error("%s", "Error: resize entered with null value\n");
        return false;

    }

    debug("List capacity: %lu, List length: %lu\n", self->capacity, self->length);

    if(self->length == self->capacity){

        //

    }else if(self->length == (self->capacity/2)-1){

        //

    }

    return ret;
}

arraylist_t *new_al(size_t item_size){

    arraylist_t *ret = NULL;
    ret = calloc(1, sizeof(arraylist_t));

    if(ret == NULL)
    {

        fprintf(stderr, "Error: out of memory\n");
        errno = ENOMEM;
        return NULL;

    }

    ret->capacity =     INIT_SZ;
    ret->length =       0;
    ret->item_size =    item_size;
    ret->base =         calloc(INIT_SZ, item_size);

    if(ret->base == NULL){

        fprintf(stderr, "Error: out of memory\n");
        errno = ENOMEM;
        return NULL;

    }

    return ret;
}

size_t insert_al(arraylist_t *self, void* data){
    size_t ret = UINT_MAX;

    resize_al(self);

    return ret;
}

void *get_data_al(arraylist_t *self, void *data){
    void *ret = NULL;

    return ret;
}
void *get_index_al(arraylist_t *self, size_t index){
    void *ret = NULL;

    return ret;
}

void *remove_data_al(arraylist_t *self, void *data){
    void *ret = 0;

    resize_al(self);

    return ret;
}
void *remove_index_al(arraylist_t *self, size_t index){
    void *ret = 0;

    resize_al(self);

    return ret;
}

void delete_al(arraylist_t *self, void (*free_item_func)(void*)){

    free_item_func(self);
    free(self);

    return;
}
