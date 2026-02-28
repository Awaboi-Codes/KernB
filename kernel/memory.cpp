#include "memory.h"

const unsigned int heapSize = 1024 * 1024; // in bytes
unsigned int blockSize = 256; // 256 bytes
unsigned char heap[heapSize]; // 1MB heap
unsigned int  heap_pos = 0;

unsigned int blocks = heapSize / blockSize;

void* allocate_block() {
    for (int i = 0; i < blocks; i++) {
        unsigned char* block_start = &heap[i * blockSize];
        if (block_start[0] == 0) {      // free?
            block_start[0] = 1;         // mark used
            return (void*)(block_start + 1); // return usable data ptr
        }
    }
    return 0; // out of memory
}

void free_block(void* ptr) {
    if (!ptr) return;
    unsigned char* block_start = (unsigned char*)ptr - 1;
    block_start[0] = 0; // mark free
}

void* kmalloc(unsigned int size) {
    size = (size + 3) & ~3; // align to 4 bytes
    if (heap_pos + size > sizeof(heap)) return 0;
    void* ptr = &heap[heap_pos];
    heap_pos += size;
    return ptr;
}

void kfree(void* ptr) {
    (void)ptr; // bump allocator — can't free individual blocks yet
}

unsigned int kmem_used() { return heap_pos; }
