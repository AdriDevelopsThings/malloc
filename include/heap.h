#ifndef HEAP_H
#define HEAP_H

#include <stddef.h>
typedef struct heap_ heap;
#include "block.h"


struct heap_ {
	size_t real_size;
	size_t size;
	size_t free;
	size_t block_free;
	size_t used_blocks;

	block* block;
	
	struct heap_* previous;
	struct heap_* next;
};

heap* new_heap(size_t size);
int free_heap(heap* f_heap);


#endif
