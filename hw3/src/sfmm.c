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

	int finalBlockSize = roundup((double)size/16)*16;

	info("Size requested: %d\n", (int)size);
	info("Block size calculated: %d\n", finalBlockSize);

	if(freelist_head == NULL){
		//Empty list. Nothing has been allocated yet




	}else{

		//Freelist already exists


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

void dummy(void* dummy){}