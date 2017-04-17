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

    if(self == NULL){

        error("%s", "Resize entered with null value\n");
        errno = EINVAL;
        return false;

    }else

    debug("List: %p\n", SHORT_ADDR(self));
    debug("List capacity: %lu, List length: %lu\n", self->capacity, self->length);
    debug("Current total list size: %lu\n", self->capacity * self->item_size);

    if(self->length == self->capacity){

        //Grow!
        debug("Attempting to grow to capacity: %lu, size: %lu\n", self->capacity*2, (self->capacity)*2*(self->item_size));
        void *new_base = calloc(self->capacity*2, self->item_size);
        void *old_base = self->base;

        if(new_base == NULL){

            error("List %p cannot be resized", SHORT_ADDR(self));
            return false;

        }

        memmove(new_base, old_base, self->length * self->item_size);

        free(old_base);
        self->base = new_base;
        self->capacity *= 2;

        debug("Final list capacity: %lu, List length: %lu\n", self->capacity, self->length);

        return true;


    }else if(self->length == (self->capacity/2)-1 && (self->capacity/2) > INIT_SZ){

        //Shrink!
        debug("Attempting to shrink to capacity: %lu\n", self->capacity/2);
        void *new_base = calloc(self->capacity/2, self->item_size);
        void *old_base = self->base;

        if(new_base == NULL){

            error("List %p cannot be resized", SHORT_ADDR(self));
            return false;

        }

        memmove(new_base, old_base, self->length * self->item_size);

        free(old_base);
        self->base = new_base;
        self->capacity /= 2;

        debug("Final list capacity: %lu, List length: %lu\n", self->capacity, self->length);

        return true;

    }

    return true;

}

/*****************************************************/
arraylist_t *new_al(size_t item_size){

    debug("Entered new_al with size: %lu\n", item_size);
    debug("Initial size: %d\n", INIT_SZ);
    debug("Current total list size: %lu\n", item_size * INIT_SZ);

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
    if(!resize_al(self)){

        error("%s","Resize failed\n");
        return -1;

    }

    debug("Base address after resize: %p\n", SHORT_ADDR(self->base));

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
    size_t ret = UINT_MAX;

    //Base case checks
    if(self == NULL){
        errno = EINVAL;
        return ret;
    }

    if(data == NULL){

        ret = 0;
        return ret;

    }

    size_t itemsize =   self->item_size;
    void *baseaddr =    self->base;
    int index =         -1;

    //Iterate over the list until a match is found or we are out of the list
    while(memcmp((char*)baseaddr + (itemsize * ++index), data, itemsize) != 0 && index < self->length)
        debug("Checked memory address: %p\n", (char*)baseaddr + (itemsize * index));

    if(index < self->length){

        debug("Found match at index: %d\n", index);
        ret = index;

    }else{

        //No match could be found
        debug("%s","No match found\n");

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

    if(free_item_func != NULL){
        //The free function isn't null

        debug("%s\n","Custom free function passed");

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

    }else{

        debug("%s","No custom freeing function passed...\n");

    }

    debug("Freeing base array: %p\n", SHORT_ADDR(self->base));
    free(self->base);
    debug("Freeing arraylist reference: %p\n", SHORT_ADDR(self));
    free(self);

    return;
}
