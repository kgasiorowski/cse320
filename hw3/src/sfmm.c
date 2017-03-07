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

void *sf_malloc(size_t size) {

	const size_t PAGE = 4096;

	if(size > 4*PAGE){
		errno = EINVAL;
		return NULL;
	}else if(size == 0)
		return NULL;

	int requestedSize = (int)size;
	int finalPayloadSize = roundup((double)size/16)*16;
	int finalBlockSize = finalPayloadSize+SF_HEADER_SIZE+SF_FOOTER_SIZE;
	int shiftedFinalBlockSize = finalBlockSize>>4;

	debug("Size requested: %d\n", requestedSize);
	debug("Payload size calculated: %d\n", finalPayloadSize);
	debug("Block size calculated: %d\n", finalBlockSize);

	sf_free_header* cursor = freelist_head;
	dummy(cursor);

	if(freelist_head == NULL){
		//Empty list. All blocks are currently allocated.
		//Ask for more heap space and return that address+1.
		debug("%s", "No free blocks detected.\n");
		debug("Final block size: %d\n", finalBlockSize);

		void *baseBlockLocation = sf_sbrk(finalBlockSize);

		sf_header* header =  baseBlockLocation;
		debug("Header address: %p\n", (void*)header);
		header->alloc = 1;

		debug("Setting header requested_size = %d\n", requestedSize);
		header->requested_size = requestedSize;

		debug("Setting header padding_size == %d - %d = %d\n", finalPayloadSize, requestedSize, finalPayloadSize-requestedSize);
		header->padding_size = finalPayloadSize - requestedSize;

		header->block_size = shiftedFinalBlockSize;
		debug("Setting header block size = %d\n", header->block_size);

		//Set footer

		sf_footer* footer = (void*)((char*)baseBlockLocation+finalBlockSize-SF_FOOTER_SIZE);
		debug("Footer address: %p\n", (void*)footer);

		debug("Difference: %lu\n", (char*)footer-(char*)header);

		footer->alloc = 1;
		footer->block_size = shiftedFinalBlockSize;

		//Split this block after the footer

		//This now holds the address directly after the footer of the above block
		sf_free_header* newFreeBlock = (void*)((char*)baseBlockLocation+finalBlockSize);

		newFreeBlock->next = NULL;
		newFreeBlock->prev = NULL;

		newFreeBlock->header.alloc = 0;

			//Add the head right after
			//Set the footer
			//Add it to the free block linked list

		//Return this block after the header
		return (sf_header*)baseBlockLocation+1;


	}else{

		//Freelist exists. Find the best fit block by iterating through the list.



	}


	return NULL;

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

	if(intvalue == input){

		return intvalue;

	}else{

		return intvalue+1;

	}

}

void insert_into_freelist(){



}

void dummy(void* dummy){}