#ifndef BLOCK_H
#define BLOCK_H

#include <stddef.h>
typedef struct block_ block;
#include "heap.h"

struct block_ {
    size_t real_size;
	size_t size;
	void* pointer;
	int freed;
	heap* associated_heap;

	struct block_* next;
};

block* reduce_block_size(block* f_block, size_t size);
block* add_block(heap* c_heap, block* last_block, size_t size);
void remove_block(block* r_block);
void auto_merge_blocks(block* r_block);
void extend_block(block* e_block, size_t size);

#endif