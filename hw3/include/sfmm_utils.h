#ifndef SFMM_UTILS_H
#define SFMM_UTILS_H

#define PAGE_SIZE (4096.0)
#define LINE_SIZE (16.0)

#define SHORT_ADDR(V) ((void*)(((unsigned long)V<<48)>>48))

void dummy(void*);
int roundup(double);
void *allocate_from_free_block(sf_free_header*, size_t);
void insert_into_freelist(sf_free_header*);
sf_free_header* find_match(size_t);
sf_free_header *coalesce(sf_free_header*);
int freelist_length();
sf_footer *get_footer(sf_header*);
sf_header *get_header(sf_footer*);

#endif
