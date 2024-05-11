#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>

typedef struct {
	char* head;
	char* end;
} Mempool;

Mempool* mempool_create(size_t size);
void* mempool_alloc(Mempool* pool, size_t size);
void mempool_destroy(Mempool *pool);
size_t mempool_remaining(Mempool *pool);

#endif
