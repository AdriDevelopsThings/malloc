#include "heap.h"
#include "block.h"

// returns the pointer to a new block if a new block is created otherwise NULL
block* reduce_block_size(block* f_block, size_t size) {
    f_block->associated_heap->free -= (f_block->size - size);
    if (f_block->next == NULL) {
        f_block->associated_heap->block_free -= (f_block->size - size);
    }
    
    f_block->size = size;
    f_block->real_size = size + sizeof(f_block);

    // f_block isn't the last block, so a block between f_block and the (old) next block must be created
    if (f_block->next != NULL) {
        block* next = f_block->next;
        block* n_block = add_block(f_block->associated_heap, f_block, (f_block->size - size));
        n_block->next = next;
        return n_block;
    }
    return NULL;
}

void extend_block(block* e_block, size_t size) {
    e_block->associated_heap->free -= (size - e_block->size);
    e_block->associated_heap->block_free -= (size - e_block->size);
    e_block->real_size = size + sizeof(block);
    e_block->size = size;
}

block* add_block(heap* c_heap, block* last_block, size_t size) {
	block* p_block;
	if (last_block == NULL) {
		p_block = (block*) c_heap + sizeof(heap);
	} else {
		p_block = last_block + sizeof(block) + last_block->real_size;
	}

	p_block->associated_heap = c_heap;
    p_block->real_size = size + sizeof(block);
	p_block->size = size;
	p_block->freed = 0;
	p_block->pointer = p_block + sizeof(block);
    p_block->next = NULL;
	if (last_block != NULL) {
		last_block->next = p_block;
	}

	c_heap->free -= size + sizeof(block);
	c_heap->block_free -= size + sizeof(block);
    c_heap->used_blocks++;
	return p_block;
}

void merge_blocks(block* m_block) {
    block* next = m_block->next;
    m_block->next = next->next;
    m_block->real_size += next->real_size;
    m_block->size += next->real_size;
    m_block->associated_heap->free += sizeof(block);
    m_block->associated_heap->used_blocks--;
}

void auto_merge_blocks(block* r_block) {
    // merge freed blocks together
    while (r_block->next != NULL && r_block->next->freed) {
        merge_blocks(r_block);
        r_block = r_block->next;
    }
}

void remove_block(block* r_block) {
	r_block->freed = 1;
	r_block->associated_heap->free += r_block->size + sizeof(block);
    r_block->associated_heap->used_blocks--;

    // check if memory page (heap) is empty
	if (r_block->associated_heap->used_blocks == 0) {
		heap* previous = r_block->associated_heap->previous;
		heap* next = r_block->associated_heap->next;
		free_heap(r_block->associated_heap);
		if (previous != NULL) {
			previous->next = next;
		}
	} else {
        auto_merge_blocks(r_block);
    }
}