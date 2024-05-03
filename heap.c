#include "heap.h"
#include <sys/mman.h>
#include <unistd.h>
#define PAGESIZE sysconf(_SC_PAGESIZE)
#define MINIMAL_PAGESIZE 4


heap* new_heap(size_t size) {
	size_t heap_size = MINIMAL_PAGESIZE * PAGESIZE;

	// the heap size should include space for a heap struct and a block
	if (size + sizeof(heap) + sizeof(block) > heap_size) {
		heap_size = (size + sizeof(heap) + sizeof(block) / PAGESIZE) + 1;
	}

	heap* n_heap = (heap*) mmap(NULL, heap_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

	if (n_heap == NULL) {
		return NULL;
	}

	n_heap->real_size = heap_size;
	n_heap->size = heap_size - sizeof(heap);
	n_heap->free = heap_size - sizeof(heap);
	n_heap->block_free = heap_size - sizeof(heap);
	n_heap->used_blocks = 0;
	n_heap->block = NULL;
	n_heap->next = NULL;
	return n_heap;
}

int free_heap(heap* f_heap) {
	return munmap(f_heap, f_heap->real_size);
}
