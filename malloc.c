#include "heap.h"
#include "memcopy.h"
#include <unistd.h>

heap* first_heap = NULL;

void* malloc(size_t size) {
	// create the first heap if there is no heap
	if (first_heap == NULL) {
		first_heap = new_heap(size);
		if (first_heap == NULL) {
			return NULL;
		}
	}

	// iterate through heaps
	heap* c_heap = first_heap;
	block* found_block;
	while (1) {
		// the current heap COULDN'T contain space for a new block
		if (c_heap->free < size + sizeof(block)) {
			// there are no heaps anymore, create a new one
			if (c_heap->next == NULL) {
				heap* n_heap = new_heap(size);
				if (n_heap == NULL) {
					return NULL;
				}
				c_heap->next = n_heap;
				c_heap = n_heap;
				c_heap->block = add_block(c_heap, NULL, size);
				found_block = c_heap->block;
				break; // a heap that has enough space for the block has been found
			} else {
				// try the next one
				c_heap = c_heap->next;
			}
		} else if (c_heap->block == NULL) { // this heap has enough space and does not contain any block
			c_heap->block = add_block(c_heap, NULL, size);
			found_block = c_heap->block;
			break;
		} else { // this heap COULD have enough space for the block
			// iterate through the blocks
			block* c_block = c_heap->block; // first block
			for (block* c_block=c_heap->block; !c_block->freed || c_block->size < size; c_block = c_block->next) {
				if (c_block->next == NULL) { // this is the last block
					if (c_heap->block_free < size + sizeof(block)) { // there is no space for a new block
						c_block = NULL; // invalidate the current block and stop iterating
					} else  { // there is enough space for a new block
						block *n_block = add_block(c_heap, c_block, size);
						c_block = n_block; // found a new block
					}
					break;
				}
			}
			if (c_block != NULL) { // this heap contains space, found_block contains the new block
				found_block = c_block;
				c_block->freed = 0;
				if (c_block->size > size) {
					reduce_block_size(c_block, size);
				}
				break;
			}
		}
	}
	if (found_block == NULL) { // no block was found
		return NULL;
	}
	return found_block->pointer;
}

block* get_block_ptr(void* ptr) {
	if (first_heap == NULL) {
		return NULL;
	}
	for (heap* c_heap=first_heap; c_heap->next != NULL;c_heap=c_heap->next) {
		if ((void*) c_heap < ptr && (void*) (c_heap + c_heap->real_size) > ptr) { // ptr is in c_heap
			for (block* c_block; c_block != NULL; c_block=c_block->next) {
				if (c_block->pointer == ptr) {
					return c_block;
				}
			}
			break; // another heap couldn't contain ptr
		}
	}
	return NULL;
}

void* realloc(void* ptr, size_t size) {
	block* r_block = get_block_ptr(ptr);
	if (r_block == NULL) {
		return NULL;
	}
	size_t o_size = r_block->size;

	if (r_block->next == NULL) {
		if (r_block->associated_heap->block_free >= (size - r_block->size)) {
			extend_block(r_block, size);
			return ptr;
		}
	} else if (r_block->associated_heap->free > size - o_size && r_block->next->freed) {
		auto_merge_blocks(r_block);
		if (r_block->size >= size) {
			if (r_block->size > size) {
				reduce_block_size(r_block, size);
			}
			return ptr;
		} 
	}

	void* new_ptr = malloc(size);
	if (new_ptr == NULL) {
		return NULL;
	}

	memcopy(ptr, new_ptr, o_size);

	remove_block(r_block);
}

void free(void* ptr) {
	block* block = get_block_ptr(ptr);
	if (block != NULL) {
		remove_block(block);
	}
}
