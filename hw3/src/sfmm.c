/**
 * All functions you make for the assignment must be implemented in this file.
 * Do not submit your assignment with a main function in this file.
 * If you submit with a main function in this file, you will get a zero.
 */
#include "sfmm.h"
#include "sfmm_utils.h"
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <debug.h>

/**
 * You should store the head of your free list in this variable.
 * Doing so will make it accessible via the extern statement in sfmm.h
 * which will allow you to pass the address to sf_snapshot in a different file.
 */
sf_free_header* freelist_head = NULL;

//TODO - IMPLEMENT ME
void *sf_malloc(size_t size) {

	if(size > 4*PAGE_SIZE){
		errno = EINVAL;
		return NULL;
	}else if(size == 0)
		return NULL;

	//Calculate the total block size of what the user is requesting.
	size_t totalBlockSize = (roundup(size/LINE_SIZE)*LINE_SIZE)+SF_FOOTER_SIZE+SF_HEADER_SIZE;
	sf_free_header *block_to_alloc = find_match(size);

	//Freelist is empty, which means theres no free space. We must ask for more.
	if(block_to_alloc == NULL){

		void *baseBlockLocation = sf_sbrk(totalBlockSize);

		//Return error if
		if(baseBlockLocation == NULL){
			errno = ENOMEM;
			return NULL;
		}

		size_t new_block_size = ((char*)sf_sbrk(0) - (char*)baseBlockLocation);
		debug("Calculated block size asked from heap: %d\n", (int)new_block_size);
		sf_free_header* free_page = baseBlockLocation;

		free_page->header.alloc=0;
		free_page->header.block_size = new_block_size>>4;

		sf_footer *new_page_footer = (void*)((char*)baseBlockLocation+new_block_size-SF_FOOTER_SIZE);
		new_page_footer->alloc = 0;
		new_page_footer->splinter = 0;
		new_page_footer->block_size = new_block_size>>4;

		insert_into_freelist(free_page);
		debug("Freelist length: %d\n", freelist_length());
		block_to_alloc = find_match(size);

	}

	return allocate_from_free_block(block_to_alloc, size);

}

void *sf_realloc(void *ptr, size_t size) {
	return NULL;
}

void sf_free(void* ptr) {
	return;
}

int sf_info(info* ptr) {
	return -1;
}

////////////////////////////////////////////////////////
//
// Utils
//
////////////////////////////////////////////////////////
int roundup(double input){

	int intvalue = (int)input;

	if(intvalue == input)
		return intvalue;
	else
		return intvalue+1;

}

//Returns null if no matching block can be found.
sf_free_header* find_match(size_t requested_size){

	debug("Entered find match with size: %d\n", (int)requested_size);

	sf_free_header* cursor = freelist_head;
	sf_free_header* closest_match = NULL;
	int smallest_difference = PAGE_SIZE*100;

	int difference = 0;
	size_t current_block_size = -1;
	size_t requested_block_size = (roundup(requested_size/LINE_SIZE)*LINE_SIZE)+SF_FOOTER_SIZE+SF_HEADER_SIZE;
	debug("Requested block size: %d\n", (int)requested_block_size);

	//Iterate through the linked list
	while(cursor != NULL){

		//Get the current block size
		current_block_size = cursor->header.block_size<<4;
		debug("Current block size: %d\n", (int)current_block_size);

		//Find the difference between the current block size, and the
		//needed block size
		difference = current_block_size-requested_block_size;
		debug("Difference calculated: %d\n", (int)difference);

		//Move on if the block is too small
		if(difference < 0)
			continue;

		//If a perfect match was found,
		//Immediately return it
		if(difference == 0){
			closest_match = cursor;
			return closest_match;
		}

		//If this difference was the smallest so far,
		//Set it as the smallest difference.
		if(difference < smallest_difference){
			smallest_difference = difference;
			closest_match = cursor;
		}

		//Iterate
		cursor = cursor->next;
	}

	debug("Returning closest_match: (%p)\n", closest_match);
	return closest_match;

}

//Removes a free block from the free list
//Essentially a deletion from a linked list
void remove_from_freelist(sf_free_header* block_to_remove){

	sf_free_header *cursor = freelist_head;

	debug("Entered remove with (%p)\n", (void*)block_to_remove);

	while(cursor != NULL && cursor != block_to_remove){

		cursor = cursor->next;

	}

	//List is empty or node not found.
	if(cursor == NULL)
		return;

	//Delete the only node
	if(cursor->next == NULL && cursor->prev == NULL)
	{

		freelist_head = NULL;
		return;

	}

	//Delete tail
	if(cursor->next == NULL)
	{

		cursor->prev->next = NULL;
		cursor->prev = NULL;
		return;

	}

	//Delete head
	if(cursor->prev == NULL)
	{

		freelist_head = cursor->next;
		cursor->next->prev = NULL;
		cursor->next = NULL;
		return;

	}

	//Delete mid-list
	cursor->prev->next = cursor->next;
	cursor->next->prev = cursor->prev;
	cursor->next = NULL;
	cursor->prev = NULL;
	return;

}

//Inserts a free block into the free list, sorted by address ascending
void insert_into_freelist(sf_free_header* block_to_insert){

	debug("Entered insertion function with (%p) with block size (%d)\n", (void*)block_to_insert, block_to_insert->header.block_size<<4);

	sf_free_header *cursor = freelist_head;

	//Insert in front
	if(freelist_head == NULL){

		debug("Inserted free block (%p) at the beginning of the list\n", (void*)block_to_insert);
		freelist_head = block_to_insert;
		return;

	}

	while(block_to_insert > cursor){

		if(cursor->next != NULL)
			cursor = cursor->next;
		else{
			//Insert at the end
			debug("Inserted block (%p) at the end\n", (void*)block_to_insert);
			cursor->next = block_to_insert;
			block_to_insert->prev = cursor;
			block_to_insert->next = NULL;
			return;

		}

	}

	debug("Inserted block (%p) in the middle of the list\n", (void*)block_to_insert);

	//Insert in the middle
	cursor->prev->next = block_to_insert;
	block_to_insert->prev = cursor;

	cursor->prev = block_to_insert;
	block_to_insert->next = cursor;

	return;

}

//Coalesces a free block with any surrounding free blocks
//TODO
sf_free_header *coalesce(sf_free_header* block_to_coalesce){

	return NULL;

}

//Takes a free block, and cuts it into two pieces:
//One allocated block which is returned, and one free
//block which is re-inserted into the freeblock linked list
//
//Returns the address directly after the new allocated header
//
//Reminder: All block sizes must be shifted right twice
void *allocate_from_free_block(sf_free_header* freeblock, size_t requested_size){

	debug("Requested size: %d\n", (int)requested_size);
	sf_free_header *free_header = freeblock;
	sf_footer *original_free_footer = (void*)((char*)freeblock + free_header->header.block_size - SF_FOOTER_SIZE);
	debug("Free block footer address: %p\n", (void*)original_free_footer);
	size_t original_free_block_size = free_header->header.block_size<<4;
	sf_header *alloc_header = (void*)freeblock;

	//Remove the free block since we're about to allocate some of it
	remove_from_freelist(freeblock);

	//Declare and initialize the values we need
	size_t payload_size;
	size_t padding_size;
	size_t block_size;

	payload_size = roundup(requested_size/LINE_SIZE)*LINE_SIZE;
	padding_size = payload_size - requested_size;
	block_size = payload_size + SF_FOOTER_SIZE + SF_HEADER_SIZE;

	debug("Calculated payload size: %d\n", (int)payload_size);
	debug("Calculated padding size: %d\n", (int)padding_size);
	debug("Calculated block size: %d\n", (int)block_size);

	debug("Alloc_header address: %p\n", (void*)alloc_header);
	//Initialize the new allocated values in the header
	alloc_header->alloc = 1;
	alloc_header->splinter = 0;
	alloc_header->block_size = block_size>>4;
	alloc_header->requested_size = requested_size;
	alloc_header->splinter_size = 0;
	alloc_header->padding_size = padding_size;

	//Initialize the footer
	sf_footer *alloc_footer = (void*)((char*)alloc_header+block_size-SF_FOOTER_SIZE);
	debug("Alloc footer address: %p\n", (void*)alloc_footer);
	alloc_footer->alloc = 1;
	alloc_footer->splinter = 0;
	alloc_footer->block_size = block_size>>4;

	//Create a new free header for the block we are carving up
	sf_free_header *newFreeBlock = (void*)((alloc_footer)+1);
	//Calculate the new free block size
	size_t new_free_block_size = (original_free_block_size - block_size)>>4;

	debug("New free header address: %p\n", (void*)newFreeBlock);
	newFreeBlock->header.alloc = 0;
	newFreeBlock->header.splinter = 0;
	newFreeBlock->header.block_size = new_free_block_size;
	newFreeBlock->header.requested_size = 0;
	newFreeBlock->header.splinter_size = 0;
	newFreeBlock->header.padding_size = 0;

	//Update the fields in the old free block footer
	original_free_footer->block_size = new_free_block_size;
	debug("Free block footer address: %p\n", (void*)original_free_footer);

	insert_into_freelist(newFreeBlock);
	debug("Freelist length: %d\n", freelist_length());

	return (void*)(alloc_header+1);

}

//Returns the length of the free list.
int freelist_length(){

	int i = 0;

	sf_free_header *cursor = freelist_head;

	while(cursor != NULL){

		i++;
		cursor = cursor->next;

	}

	return i;

}

//Serves no purpose other than to get rid of
//Unused variable warnings
void dummy(void* dummy){}
