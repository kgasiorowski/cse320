#include "debug.h"
#include "arraylist.h"
#include "foreach.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>

static __thread foreach_t *foreach_data;

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

    return get_index_al(self, 0);

}

void *foreach_next(arraylist_t *self, void *data){

    //Indicates end of iterator
    if(data == NULL)
        return NULL;

    void *rtn;
    size_t current_index;

    current_index = get_data_al(self, data);

    if(current_index == self->length)
        rtn = NULL;
    else
        rtn = get_index_al(self, current_index+1);

    foreach_data->current_index++;

    return rtn;

}

size_t foreach_index(){

    if(foreach_data == NULL)
        return UINT_MAX;
    else
        return foreach_data->current_index;

}

bool foreach_break_f(){

    free(foreach_data);

    return true;

}

int32_t apply(arraylist_t *self, int32_t (*application)(void*)){

    return 0;

}
