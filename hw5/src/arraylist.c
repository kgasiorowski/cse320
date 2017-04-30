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

    }

    debug("List: %p\n", SHORT_ADDR(self));
    debug("List capacity: %lu, List length: %lu\n", self->capacity, self->length);
    debug("Current total list size: %lu\n", self->capacity * self->item_size);

    //WRITER SECTION
    sem_wait(&self->write_lock);
    if(self->length == self->capacity){

        //Grow!
        debug("Attempting to grow to capacity: %lu, size: %lu\n", self->capacity*2, (self->capacity)*2*(self->item_size));
        self->base = realloc(self->base, self->capacity * self->item_size * 2);

        if(self->base == NULL){

            error("List %p cannot be resized", SHORT_ADDR(self));
            sem_post(&self->write_lock);
            return false;

        }
        self->capacity *= 2;

        debug("Final list capacity: %lu, List length: %lu\n", self->capacity, self->length);

    }else if(self->length == (self->capacity/2)-1 && (self->capacity/2) >= INIT_SZ){

        //Shrink!
        debug("Attempting to shrink to capacity: %lu\n", self->capacity/2);
        self->base = realloc(self->base, self->capacity * self->item_size / 2);

        if(self->base == NULL){

            error("List %p cannot be resized", SHORT_ADDR(self));
            sem_post(&self->write_lock);
            return false;

        }
        self->capacity /= 2;

        debug("Final list capacity: %lu, List length: %lu\n", self->capacity, self->length);

    }
    sem_post(&self->write_lock);
    //WRITER SECTION ENDS

    return true;

}

/*****************************************************/
//Multithreading not necessary
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
    sem_init(&ret->lock, 0, 1);
    sem_init(&ret->write_lock, 0, 1);
    ret->readcnt = 0;

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
//WRITER - implemented
size_t insert_al(arraylist_t *self, void* data){

    debug("%s\n","Entered insert");

    //Base case checks
    if(self == NULL || data == NULL){
        errno = EINVAL;
        return UINT_MAX;
    }

    //Since the size is changing, resize the list as needed
    if(!resize_al(self)){

        error("%s","Resize failed\n");
        return UINT_MAX;

    }

    debug("Base address after resize: %p\n", SHORT_ADDR(self->base));

    //Lock!
    sem_wait(&self->write_lock);

    size_t offset = (self->item_size)*(self->length);
    debug("Offset calculated from index %lu and size %lu: %lu\n", self->length, self->item_size, offset);

    //Calculate the new offset to write to and copy the data
    void *new_index_location = (char*)self->base + offset;

    debug("Location of new item: %p\n", SHORT_ADDR(new_index_location));

    memcpy(new_index_location, data, self->item_size);

    //Increase the length of the list since we are inserting
    self->length++;

    sem_post(&self->write_lock);
    //Unlock!

    debug("New list length: %lu, index returned: %d\n", self->length, (int)(self->length-1));

    //Return the index of the thing (should be last)
    return self->length-1;
}

//READER - implemented
size_t get_data_al(arraylist_t *self, void *data){
    //size_t ret = UINT_MAX;

    //Base case checks
    if(self == NULL){
        errno = EINVAL;
        return UINT_MAX;
    }

    if(data == NULL){

        return 0;

    }

    size_t itemsize =       self->item_size;
    void *baseaddr =        self->base;
    int index;
    void *current_item =    NULL;

    //READER SECTION
    sem_wait(&self->lock);
    self->readcnt++;
    if(self->readcnt == 1) //First in
        sem_wait(&self->write_lock);

    sem_post(&self->lock);

    //Iterate over the list until a match is found or we are out of the list
    for(index = 0; index < self->length; index++){

        current_item = (char*)baseaddr + (index * itemsize);
        int cmp = memcmp(current_item, data, itemsize);
        debug("%p : %d\n", SHORT_ADDR(current_item), cmp);

        if(!cmp)
            break;

    }

    sem_wait(&self->lock);
    self->readcnt--;
    if(self->readcnt == 0)
        sem_post(&self->write_lock);

    sem_post(&self->lock);
    //READER SECTION ENDS

    if(index < self->length){

        debug("Found match at index: %d\n", index);
        return index;

    }else{

        //No match could be found
        debug("%s","No match found\n");
        return UINT_MAX;

    }

}

/*****************************************************/
//READER - implemented
void *get_index_al(arraylist_t *self, size_t index){


    if(self == NULL || index >= self->length){
        errno = EINVAL;

        return NULL;
    }


    void *ret = calloc(1, self->item_size);
    debug("Return address: %p\n", SHORT_ADDR(ret));
    if(ret == NULL){

        error("%s","Copy could not be created\n");
        return NULL;

    }

    //READING SECTION
    sem_wait(&self->lock);
    self->readcnt++;
    if(self->readcnt == 1) //First in
        sem_wait(&self->write_lock);

    sem_post(&self->lock);

    //Values
    void *baseaddr = self->base;
    size_t itemsize = self->item_size;

    debug("Base address: %p\n", SHORT_ADDR(baseaddr));
    debug("Item size: %lu\n", itemsize);

    void *itemloc = (char*)baseaddr + (itemsize*index);

    debug("Final item location: %p\n", SHORT_ADDR(itemloc));

    memcpy(ret, itemloc, itemsize);
    sem_wait(&self->lock);
    self->readcnt--;
    if(self->readcnt == 0) //Last out
        sem_post(&self->write_lock);

    sem_post(&self->lock);
    //READING SECTION ENDS

    debug("%s\n", "Completed memory copy");
    debug("Returning address: %p\n", SHORT_ADDR(ret));

    return ret;

}

/*****************************************************/
//WRITER - implemented
bool remove_data_al(arraylist_t *self, void *data){

    size_t itemsize =       self->item_size;
    void *baseaddr =        self->base;
    int index;
    void *current_item =    NULL;

    //Find the index

    //WRITER SECTION
    sem_wait(&self->write_lock);
    for(index = 0; index < self->length; index++){

        current_item = (char*)baseaddr + (index * itemsize);
        debug("Checking address: %p\n", SHORT_ADDR(current_item));
        int cmp = memcmp(current_item, data, itemsize);

        debug("%p : %d\n", SHORT_ADDR(current_item), cmp);

        if(!cmp)
            break;

    }

    debug("Index of match: %d\n", index);

    void *itemaddr, *nextitemaddr;

    itemaddr =      (char*)baseaddr + (index * itemsize);
    nextitemaddr =  (char*)itemaddr + itemsize;

    while(index++ < self->length-1){

        debug("Overwriting %p with %p\n", SHORT_ADDR(itemaddr), SHORT_ADDR(nextitemaddr));
        memmove(itemaddr, nextitemaddr, itemsize);

        itemaddr =      (char*)baseaddr + (itemsize * index);
        nextitemaddr =  (char*)itemaddr + itemsize;

    }

    self->length--;
    sem_post(&self->write_lock);
    //END WRITER SECTION

    if(!resize_al(self))
        return false;

    return true;
}

/*****************************************************/
//READER and then WRITER
void *remove_index_al(arraylist_t *self, size_t index){

    debug("Entered remove with %p and index %lu\n", self, index);

    if(self == NULL)
        return NULL;

    if(self->length == 0)
        return NULL;

    sem_wait(&self->write_lock);

    // //READER SECTION
    // sem_wait(&self->lock);

    // self->readcnt++;
    // if(self->readcnt == 1) //First in
    //     sem_wait(&self->write_lock);

    // sem_post(&self->lock);

    if(index >= self->length){

        index = self->length-1;

    }

    debug("Removing index %lu\n", index);

    size_t itemsize =       self->item_size;
    void *baseaddr =        self->base;
    void *itemaddr =        (char*)baseaddr + (itemsize * index);
    void *nextitemaddr =    (char*)itemaddr + itemsize;

    void *ret = malloc(sizeof(itemsize));
    if(ret == NULL)
    {

        error("%s\n","Cannot copy element: out of memory!");
        return NULL;

    }


    if(memmove(ret, itemaddr, itemsize) == NULL)
    {

        error("%s","Memmove failed\n");
        return NULL;

    }

    // sem_wait(&self->lock);

    // self->readcnt--;
    // if(self->readcnt == 0) //Last out
    //     sem_post(&self->write_lock);

    // sem_post(&self->lock);
    // //READER SECTION ENDS

    //WRITER SECTION BEGINS

    while(index++ < self->length-1){

        debug("Overwriting %p with %p\n", SHORT_ADDR(itemaddr), SHORT_ADDR(nextitemaddr));
        memmove(itemaddr, nextitemaddr, itemsize);

        itemaddr =      (char*)baseaddr + (itemsize * index);
        nextitemaddr =  (char*)itemaddr + itemsize;

    }

    self->length--;
    sem_post(&self->write_lock);
    //WRITER SECTION ENDS

    if(!resize_al(self)){
        return NULL;
    }

    return ret;
}

/*****************************************************/
//Not thread safe and not intended to be thread safe
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

    sem_destroy(&self->lock);
    sem_destroy(&self->write_lock);

    debug("Freeing base array: %p\n", SHORT_ADDR(self->base));
    free(self->base);

    return;
}
