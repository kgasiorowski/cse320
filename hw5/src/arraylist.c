 #include "arraylist.h"
#include "debug.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @visibility HIDDEN FROM USER
 * @return     true on success, false on failure
 */
static bool resize_al(arraylist_t* self){

    bool ret = false;

    if(self == NULL){

        error("%s", "Resize entered with null value\n");
        errno = EINVAL;
        return false;

    }else

    debug("List capacity: %lu, List length: %lu\n", self->capacity, self->length);

    if(self->length == self->capacity){

        //Grow!
        debug("Attempting to grow list to size %lu\n", self->capacity*2);

        debug("Realloc size: %lu\n", (self->capacity*2)*self->item_size);

        void *retval = realloc(self->base, (self->capacity*2)*self->item_size);
        if(retval == NULL){

            error("Could not grow list of size %lu to size %lu\n", self->capacity, self->capacity*2);
            return false;

        }

        self->capacity *= 2;
        ret = true;

    }else if(self->length == (self->capacity/2)-1){

        //Shrink!
        debug("Attempting to shrink list to size %lu\n", self->capacity/2);

        debug("Realloc size: %lu\n", (self->capacity*2)*self->item_size);

        void *retval = realloc(self->base, (self->capacity/2)*self->item_size);
        if(retval == NULL){

            error("Could not shrink list of size %lu to size %lu\n", self->capacity, self->capacity/2);
            return false;

        }

        self->capacity /= 2;
        ret = true;

    }

    debug("Final list capacity: %lu, List length: %lu\n", self->capacity, self->length);

    return ret;
}

/*****************************************************/
arraylist_t *new_al(size_t item_size){

    debug("Entered new_al with size: %lu\n", item_size);
    debug("Initial size: %d\n", INIT_SZ);

    //Base case checks
    if(item_size <= 0){
        errno = EINVAL;
        return NULL;
    }

    //Allocate memory
    arraylist_t *ret = NULL;
    ret = calloc(1, sizeof(arraylist_t));

    if(ret == NULL)
    {

        error("%s","Out of memory\n");
        errno = ENOMEM;
        return NULL;

    }

    ret->capacity =     INIT_SZ;
    ret->length =       0;
    ret->item_size =    item_size;
    ret->base =         calloc(INIT_SZ, item_size);

    if(ret->base == NULL){

        error("%s","Out of memory\n");
        errno = ENOMEM;
        return NULL;

    }

    debug("New arraylist location: %p\n", SHORT_ADDR(ret));
    debug("New base array location: %p\n", SHORT_ADDR(ret->base));

    return ret;
}

/*****************************************************/
size_t insert_al(arraylist_t *self, void* data){
    size_t ret = UINT_MAX;

    debug("%s\n","Entered insert");

    //Base case checks
    if(self == NULL || data == NULL){
        errno = EINVAL;
        return ret;
    }

    //Since the size is changing, resize the list as needed
    resize_al(self);

    size_t offset = (self->item_size)*(self->length);
    debug("Offset calculated from index %lu and size %lu: %lu\n", self->length, self->item_size, offset);

    //Calculate the new offset to write to and copy the data
    void *new_index_location = (char*)self->base + offset;

    debug("Location of new item: %p\n", SHORT_ADDR(new_index_location));
    memcpy(new_index_location, data, self->item_size);

    //Increase the length of the list since we are inserting
    self->length++;
    debug("New list length: %lu, index returned: %d\n", self->length, (int)(self->length-1));

    //Return the index of the thing (should be last)
    return self->length-1;
}


size_t get_data_al(arraylist_t *self, void *data){
    size_t ret = 0;

    //Base case checks
    if(self == NULL || data == NULL){
        errno = EINVAL;
        return ret;
    }

    size_t itemsize =   self->item_size;
    void *baseaddr =    self->base;
    int index =         -1;

    //Iterate over the list until a match is found or we are out of the list
    while(memcmp((char*)baseaddr + (itemsize * ++index), data, itemsize) != 0 && index < self->length)
        debug("Checked memory address: %p\n", (char*)baseaddr + (itemsize * index));

    if(index < self->length){

        //A match was found
        debug("Get data found at match at index: %d\n", index);
        void *memlocation = (char*)baseaddr+(itemsize*index);
        debug("Calculated location of item: %p\n", SHORT_ADDR(memlocation));

        //Create a copy of this item
        ret = calloc(1, itemsize);
        if(ret == NULL){

            error("%s","Copy could not be created\n");
            return NULL;

        }
        memcpy(ret, memlocation, itemsize);

    }else{

        //No match could be found
        debug("%s","No match found\n");
        ret = NULL;

    }

    return ret;
}

/*****************************************************/
void *get_index_al(arraylist_t *self, size_t index){

    if(self == NULL || index >= self->length){
        errno = EINVAL;
        return NULL;
    }

    void *ret = malloc(self->item_size);
    debug("Return address: %p\n", SHORT_ADDR(ret));
    if(ret == NULL){

        error("%s","Copy could not be created\n");
        return NULL;

    }

    memset(ret, 0, self->item_size);

    //Values
    void *baseaddr = self->base;
    size_t itemsize = self->item_size;

    debug("Base address: %p\n", SHORT_ADDR(baseaddr));
    debug("Item size: %lu\n", itemsize);

    void *itemloc = (char*)baseaddr + (itemsize*index);

    debug("Final item location: %p\n", SHORT_ADDR(itemloc));

    memcpy(ret, itemloc, itemsize);

    debug("%s\n", "Completed memory copy");
    debug("Returning address: %p\n", SHORT_ADDR(ret));

    return ret;

}

/*****************************************************/
bool remove_data_al(arraylist_t *self, void *data){
    bool ret = false;

    resize_al(self);

    return ret;
}

/*****************************************************/
void *remove_index_al(arraylist_t *self, size_t index){
    void *ret = 0;



    resize_al(self);

    return ret;
}

/*****************************************************/
void delete_al(arraylist_t *self, void (*free_item_func)(void*)){

    if(free_item_func == NULL){
        //The free function isn't null

        void *baseaddr = self->base;
        size_t itemsize = self->item_size;

        debug("Address of this list: %p\n", SHORT_ADDR(self));
        debug("Base address of this list: %p\n", SHORT_ADDR(baseaddr));

        for(int i = 0; i < self->length; i++){

            //Calculate each item location
            void *itemaddr = (char*)baseaddr + (itemsize * i);
            debug("Address of this item: %p\n", SHORT_ADDR(itemaddr));
            //Call the custom free function on each item
            free_item_func(itemaddr);

        }

    }

    free(self);

    return;
}
