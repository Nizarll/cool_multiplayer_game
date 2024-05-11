#include "../libs/utils.h"

Mempool* mempool_create(size_t size) {
	Mempool *pool = malloc(size + sizeof(Mempool));
	pool->head = (char*)&pool[1];
	pool->end = pool->head + size;
	return pool;
}

size_t mempool_remaining(Mempool *pool) {
	return pool->end - pool->head;
}

void mempool_destroy(Mempool *pool) {
	free((void*) pool);
}

void* mempool_alloc(Mempool *pool, size_t size) {
	if (mempool_remaining(pool) < size) return nullptr;
	void* mem = (void*)pool->head;
	pool->head += size;
	return mem;
}
