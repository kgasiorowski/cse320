#ifndef SFMM_UTILS_H
#define SFMM_UTILS_H

#define PAGE_SIZE (4096.0)
#define LINE_SIZE (16.0)

#define SHORT_ADDR(V) ((void*)(((unsigned long)V<<48)>>48))

#define CHECK_DIV_8(ptr, ptrname) 							\
															\
do{ 														\
	if((unsigned long)ptr % 8 != 0)						\
		warn("Pointer %s is not divisible by 8!\n", ptrname);	\
}while(0)

#define CHECK_DIV_16(ptr, ptrname) 							\
															\
do{ 														\
	if((unsigned long)ptr % 16 != 0)						\
		warn("Pointer %s is not divisible by 16!\n", ptrname);	\
}while(0)													\


void dummy(void*);
int roundup(double);
void *allocate_from_free_block(sf_free_header*, size_t);
void insert_into_freelist(sf_free_header*);
sf_free_header* find_match(size_t);
void coalesce(sf_free_header*);
int freelist_length();
sf_footer *get_footer(sf_header*);
sf_header *get_header(sf_footer*);
void freelist_info();
sf_free_header *find_in_freelist(void*);
int freelist_contains(sf_free_header*);

#endif
