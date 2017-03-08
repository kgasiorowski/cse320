#ifndef SFMM_UTILS_H
#define SFMM_UTILS_H

void dummy(void*);
int roundup(double);
void *allocate_from_free_block(sf_free_header*, size_t);
void insert_into_freelist(sf_free_header*);

#endif
