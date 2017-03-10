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

	if(size > (4*PAGE_SIZE)-SF_FOOTER_SIZE-SF_HEADER_SIZE){
		errno = EINVAL;
		error("Cannot handle the requested %lu bytes...\n", size);
		return NULL;
	}else if(size == 0)
		return NULL;

	//Calculate the total block size of what the user is requesting.
	size_t totalBlockSize = (roundup(size/LINE_SIZE)*LINE_SIZE)+SF_FOOTER_SIZE+SF_HEADER_SIZE;
	sf_free_header *block_to_alloc = find_match(size);

	debug("Block to carve up: (%p) [%lu]\n", block_to_alloc, (unsigned long)block_to_alloc);

	//Freelist is empty, which means theres no free space. We must ask for more.
	if(block_to_alloc == NULL){

		//Request more space
		void *baseBlockLocation = sf_sbrk(totalBlockSize);

		if(baseBlockLocation == (void*)-1)
		{

			error("%s","Out of memory!\n");
			return NULL;

		}

		debug("Base heap request location (%p) [%lu]\n", SHORT_ADDR(baseBlockLocation), (unsigned long)baseBlockLocation);

		//Return error if
		if(baseBlockLocation == NULL){
			errno = ENOMEM;
			return NULL;
		}

		debug("Total block size to heap from: %lu\n", totalBlockSize);

		//Calculate the new block size
		size_t new_block_size = roundup(totalBlockSize/PAGE_SIZE)*PAGE_SIZE;
		debug("Calculated block size asked from heap: %lu\n", new_block_size);

		//Initialize the header data
		sf_free_header* free_page = baseBlockLocation;
		debug("Header address: (%p) [%lu]\n", SHORT_ADDR((void*)free_page), (unsigned long)free_page);
		free_page->header.alloc=0;
		free_page->header.block_size = new_block_size>>4;
		free_page->next = NULL;
		free_page->prev = NULL;

		//Initialize footer data
		sf_footer *new_page_footer = (sf_footer*)((unsigned long)baseBlockLocation+new_block_size-SF_FOOTER_SIZE);
		debug("Footer address: (%p) [%lu]\n", SHORT_ADDR((void*)new_page_footer), (unsigned long)new_page_footer);
		new_page_footer->alloc = 0;
		new_page_footer->splinter = 0;
		new_page_footer->block_size = new_block_size>>4;

		free_page->next = NULL;
		free_page->prev = NULL;

		//The new page(s) are free for use now. Insert it into the freelist.
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

	debug("Attempting to free block (%p) [%lu]\n", SHORT_ADDR(ptr), (unsigned long)ptr);

	sf_free_header *block_to_free = (sf_free_header*)((unsigned long)ptr-SF_FOOTER_SIZE);
	sf_footer *free_footer = get_footer((sf_header*)block_to_free);

	//Initialize all the fields to zero
	block_to_free->header.alloc = 0;
	block_to_free->header.padding_size = 0;
	block_to_free->header.splinter = 0;
	block_to_free->header.splinter_size = 0;
	block_to_free->header.requested_size = 0;

	//Initialize footer fields
	free_footer->alloc = 0;
	free_footer->splinter = 0;

	debug("Header block size: %d\n", block_to_free->header.block_size<<4);
	debug("Footer block size: %d\n", free_footer->block_size<<4);

	insert_into_freelist(block_to_free);
	debug("Freelist length: %d\n", freelist_length());
	coalesce(block_to_free);

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
//Returns the footer for a given header
sf_footer *get_footer(sf_header *header){
	debug("Entered get_footer with (%p)\n", header);

	return (sf_footer*)((unsigned long)header+(header->block_size<<4)-SF_FOOTER_SIZE);

}

//Returns the header for a given footer
sf_header *get_header(sf_footer *footer){
	debug("Entered get_header with (%p)\n", footer);

	return (sf_header*)((unsigned long)footer-(footer->block_size<<4)+SF_HEADER_SIZE);

}

//Finds a free block in the freelist by memory address
//This is definitely not needed.
sf_free_header *find_in_freelist(void* ptr){

	debug("Entered find_in_freelist (%p)\n", ptr);

	sf_free_header *cursor = freelist_head;

	while(cursor != NULL){

		if((unsigned long)cursor == (unsigned long)ptr-SF_HEADER_SIZE)
			break;

		cursor = cursor -> next;

	}

	return cursor;

}

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
		debug("Current block size (%p): %d\n", SHORT_ADDR(cursor), (int)current_block_size);

		//Find the difference between the current block size, and the
		//needed block size
		difference = current_block_size-requested_block_size;
		debug("Difference calculated: %d\n", (int)difference);

		//Move on if the block is too small
		if(difference < 0){
			cursor = cursor->next;
			continue;
		}

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

	debug("Returning closest_match: (%p) [%lu]\n", SHORT_ADDR(closest_match), (unsigned long)closest_match);
	return closest_match;

}

//Removes a free block from the free list
//Essentially a deletion from a linked list
void remove_from_freelist(sf_free_header* block_to_remove){

	debug("Entered remove with (%p) [%lu]\n", SHORT_ADDR((void*)block_to_remove), (unsigned long)block_to_remove);

	if(!freelist_contains(block_to_remove)){
		warn("%s","Cannot remove a node which does not exist...");
		return;
	}

	//Head to delete
	if(block_to_remove == freelist_head){

		debug("%s","Removing head...\n");
		freelist_head = freelist_head->next;

		if(freelist_head != NULL)
			freelist_head->prev = NULL;

	//Tail to delete
	}else if(block_to_remove->next == NULL){

		debug("%s","Removing tail...\n");
		block_to_remove->prev->next = NULL;

	//It's somewhere in the middle
	}else{

		debug("%s", "Removing somewhere in the middle of the list...\n");
		block_to_remove->next->prev = block_to_remove->prev;
		block_to_remove->prev->next = block_to_remove->next;


	}

	block_to_remove->next = NULL;
	block_to_remove->prev = NULL;

	error("%s", "!!!FREELIST AFTER REMOVE!!!");
	freelist_info();

}

int freelist_contains(sf_free_header *block){

	debug("Entered freelist contains with (%p)\n", block);

	sf_free_header *cursor = freelist_head;

	while(cursor != NULL)
		if(cursor == block)
			return 1;
		else
			cursor = cursor->next;


	return 0;

}

//Inserts a free block into the free list, sorted by address ascending
void insert_into_freelist(sf_free_header* block_to_insert){

	debug("Entered insertion function with (%p) [%lu] with block size (%d)\n", SHORT_ADDR((void*)block_to_insert), (unsigned long)block_to_insert, block_to_insert->header.block_size<<4);

	//Insert in front
	if(freelist_head == NULL){

		debug("%s","Started new freelist with block\n");
		freelist_head = block_to_insert;
		freelist_head->next=NULL;
		freelist_head->prev=NULL;

	}else if(freelist_head > block_to_insert){

		debug("%s","Inserted before head\n");
		freelist_head->prev = block_to_insert;
		block_to_insert->prev = NULL;
		block_to_insert->next = freelist_head;
		freelist_head->next = NULL;

		freelist_head = block_to_insert;

	}else{

		debug("%s","Insert somewhere after head\n");
		sf_free_header *cursor = freelist_head;
		sf_free_header *previous;

		while(cursor != NULL && cursor < block_to_insert){
			debug("Cursor: (%p)\n", SHORT_ADDR(cursor));
			debug("Previous: (%p)\n", SHORT_ADDR(previous));
			previous = cursor;
			cursor=cursor->next;
		}

		if(cursor == NULL){
		//Cursor either points at NULL - means insert at the end of the list

			debug("%s", "Insert after the end\n");
			previous->next = block_to_insert;
			block_to_insert->prev = previous;
			block_to_insert->next = NULL;

		}else{

			debug("Insert midlist before (%p) and after (%p)\n", SHORT_ADDR(cursor), SHORT_ADDR(previous));
			previous->next = block_to_insert;
			cursor->prev = block_to_insert;
			block_to_insert->next = cursor;
			block_to_insert->prev = previous;

		}

	}

	error("%s", "!!!FREELIST AFTER INSERT!!!");
	freelist_info();

}

//Coalesces a free block with any surrounding free blocks
//TODO
void coalesce(sf_free_header* block_to_coalesce){

	debug("Entered coalesce with (%p) [%lu]\n", SHORT_ADDR(block_to_coalesce), (unsigned long)block_to_coalesce);

	freelist_info();

	// return;

	sf_free_header *header_to_coalesce = block_to_coalesce;
	sf_footer *footer_to_coalesce = get_footer((sf_header*)header_to_coalesce);

	sf_footer *prev_footer = (sf_footer*)((unsigned long)header_to_coalesce-SF_HEADER_SIZE);
	sf_header *prev_header = get_header(prev_footer);

	sf_header *next_header = (sf_header*)((unsigned long)footer_to_coalesce+SF_FOOTER_SIZE);
	sf_footer *next_footer = get_footer(next_header);
	dummy(next_footer);

	int next_is_alloc = !next_header->alloc;
	int prev_is_alloc = !prev_header->alloc;

	remove_from_freelist(header_to_coalesce);

	//Check the block before it.
	debug("Previous block header addr: (%p) [%lu]\n", SHORT_ADDR(prev_header), (unsigned long)prev_header);
	debug("Previous block footer addr: (%p) [%lu]\n", SHORT_ADDR(prev_footer), (unsigned long)prev_footer);
	sf_blockprint(prev_header);

	//Check the block before it.
	//If it is free:
	if(!prev_is_alloc){

		//	remove it from the freelist
		remove_from_freelist((sf_free_header*)prev_header);
		//	add it to this block.
		//	add it to the freelist

	}




	debug("Next block header addr: (%p) [%lu]\n", SHORT_ADDR(next_header), (unsigned long)next_header);
	debug("Next block footer addr: (%p) [%lu]\n", SHORT_ADDR(next_footer), (unsigned long)next_footer);
	sf_blockprint(next_header);

	//Check the block after it.

	//If it is free:
	if(!next_is_alloc){

		//	remove it from the freelist
		remove_from_freelist((sf_free_header*)next_header);
		//	add it to this block
		//	add it to the freelist

	}

}

//Takes a free block, and cuts it into two pieces:
//One allocated block which is returned, and one free
//block which is re-inserted into the freeblock linked list
//
//Returns the address directly after the new allocated header
//
//Reminder: All block sizes must be shifted right twice
void *allocate_from_free_block(sf_free_header* freeblock, size_t requested_size){

	debug("Entered allocate_from_free_block with (%p) and %lu\n", freeblock, requested_size);
	debug("Requested size: %d\n", (int)requested_size);

	sf_free_header *free_header = freeblock;
	sf_footer *original_free_footer = get_footer((sf_header*)free_header);
	debug("Free block footer address: (%p) [%lu]\n", SHORT_ADDR((void*)original_free_footer), (unsigned long)original_free_footer);
	CHECK_DIV_16(original_free_footer, "free footer");

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

	debug("Calculated payload size: %lu\n", payload_size);
	debug("Calculated padding size: %lu\n", padding_size);
	debug("Calculated block size: %lu\n", block_size);

	debug("alloc_header address: (%p) [%lu]\n", SHORT_ADDR((void*)alloc_header), (unsigned long)alloc_header);
	CHECK_DIV_8(alloc_header, "alloc header");
	//Initialize the new allocated values in the header
	alloc_header->alloc = 1;
	alloc_header->splinter = 0;
	alloc_header->block_size = block_size>>4;
	alloc_header->requested_size = requested_size;
	alloc_header->splinter_size = 0;
	alloc_header->padding_size = padding_size;

	//Initialize the footer
	sf_footer *alloc_footer = get_footer(alloc_header);
	debug("alloc_footer address: (%p) [%lu]\n", SHORT_ADDR((void*)alloc_footer), (unsigned long)alloc_footer);
	CHECK_DIV_16((void*)alloc_footer, "New Alloc Footer");
	alloc_footer->alloc = 1;
	alloc_footer->splinter = 0;
	alloc_footer->block_size = block_size>>4;

	if(alloc_header->block_size<<4 != original_free_block_size){

		//Create a new free header for the block we are carving up
		sf_free_header *newFreeBlock = (sf_free_header*)((unsigned long)alloc_footer+SF_FOOTER_SIZE);
		//Calculate the new free block size
		size_t new_free_block_size = (original_free_block_size - block_size)>>4;

		debug("New free header address: (%p) [%lu]\n", SHORT_ADDR((void*)newFreeBlock), (unsigned long)newFreeBlock);
		CHECK_DIV_8((void*)newFreeBlock, "new free block");
		newFreeBlock->header.alloc = 0;
		newFreeBlock->header.splinter = 0;
		newFreeBlock->header.block_size = new_free_block_size;
		newFreeBlock->header.requested_size = 0;
		newFreeBlock->header.splinter_size = 0;
		newFreeBlock->header.padding_size = 0;

		//Update the fields in the old free block footer
		original_free_footer->block_size = new_free_block_size;
		debug("Free block footer address: (%p)\n", SHORT_ADDR((void*)original_free_footer));

		insert_into_freelist(newFreeBlock);
		debug("Freelist length: %d\n", freelist_length());

		debug("Final block address: (%p) [%lu]\n", SHORT_ADDR((void*)alloc_header), (unsigned long)alloc_header);
		debug("Returning address: (%p) [%lu]\n", SHORT_ADDR((void*)(alloc_header+1)), (unsigned long)(alloc_header+1));
		debug("sizeof allocated header: %lu\n", sizeof(alloc_header));

	}

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

void freelist_info(){

	sf_free_header* cursor = freelist_head;
	int i = 1;

	while(cursor != NULL){

		warn("Block #%d\n", i);
		sf_blockprint(cursor);
		i++;

		cursor = cursor->next;

	}

	warn("%s", "Exited freelist\n");

}

//Serves no purpose other than to get rid of
//Unused variable warnings
void dummy(void* dummy){}
