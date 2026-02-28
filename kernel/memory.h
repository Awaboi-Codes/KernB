#pragma once

struct block {
    int blockPosition;
};

extern unsigned char heap[1024 * 1024];
extern unsigned int  heap_pos;

void* allocate_block();
void free_block(void* ptr);
void* kmalloc(unsigned int size);
void  kfree(void* ptr);
unsigned int kmem_used();
