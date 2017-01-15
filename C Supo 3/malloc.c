#include <assert.h>
#include <string.h>
#include <stdint.h>
#include "malloc.h"
#include <stdio.h>

struct block_meta {
    size_t size;
    struct block_meta *next;
    int free;
    int magic; // For debugging only. TODO: remove this in non-debug mode.
};

static struct block_meta *find_free_block(struct block_meta **last, size_t size);
static struct block_meta *request_space(struct block_meta* last, size_t size);
static struct block_meta *get_block_ptr(void *ptr);

#define META_SIZE sizeof(struct block_meta)

static uint8_t memory[3000];
static uint8_t *top = &memory[0];
static uint8_t *bottom = &memory[0];

static void *global_base = NULL;

void *sbrk(int increment) {
    /*if (top + increment - bottom >= 3000 || increment < 0) {
        return ((void*) -1);
    }
    
    void *result;
    result = top;
    
	top += increment;
	
	return result;*/
    
    asm volatile
    (
     "movl $1, %%eax\n\t"
     "movl $1, %%edi\n\t"
     "movq %1, %%rsi\n\t"
     "movl %2, %%edx\n\t"
     "syscall"
     : "=a"(ret)
     : "g"(hello), "g"(hello_size)
     : "%rdi", "%rsi", "%rdx", "%rcx", "%r11"
    );
}

struct block_meta *find_free_block(struct block_meta **last, size_t size) {
  struct block_meta *current = global_base;
  while (current && !(current->free && current->size >= size)) {
    *last = current;
    current = current->next;
  }
  return current;
}

struct block_meta *request_space(struct block_meta* last, size_t size) {
  struct block_meta *block;
  block = sbrk(0);
  void *request = sbrk(size + META_SIZE);
  assert((void*)block == request); // Not thread safe.
  if (request == (void*) -1) {
    return NULL; // sbrk failed.
  }

  if (last) { // NULL on first request.
    last->next = block;
  }
  block->size = size;
  block->next = NULL;
  block->free = 0;
  block->magic = 0x12345678;
  return block;
}

void *malloc(size_t size) {
  struct block_meta *block;
  // TODO: align size?

  if (size <= 0) {
    return NULL;
  }

  if (!global_base) { // First call.
    block = request_space(NULL, size);
    if (!block) {
      return NULL;
    }
    global_base = block;
  } else {
    struct block_meta *last = global_base;
    block = find_free_block(&last, size);
    if (!block) { // Failed to find free block.
      block = request_space(last, size);
      if (!block) {
        return NULL;
      }
    } else {      // Found free block
      // splitting block here.
    	if (size + META_SIZE < block->size) {
    		struct block_meta *splitBlock;
            char *tempBlockPtr = (char*) block;
            tempBlockPtr = tempBlockPtr + size + META_SIZE;
    		splitBlock = (struct block_meta*) tempBlockPtr;
            
    		splitBlock->size = block->size - META_SIZE - size;
  			splitBlock->next = block->next;
 			splitBlock->free = 1;
 			splitBlock->magic = 0x92929292;

 			block->next = splitBlock;
    	}
    	

      block->free = 0;
      block->magic = 0x77777777;
    }
  }

  return(block+1);
}

struct block_meta *get_block_ptr(void *ptr) {
  return (struct block_meta*)ptr - 1;
}

void free(void *ptr) {
  if (!ptr) {
    return;
  }

  struct block_meta* block_ptr = get_block_ptr(ptr);
  assert(block_ptr->free == 0);
  assert(block_ptr->magic == 0x77777777 || block_ptr->magic == 0x12345678);


  struct block_meta *nextBlock;
  nextBlock = block_ptr->next;
  while (nextBlock != NULL && nextBlock->free == 1) {
  	//merge this free block with the next one
  	block_ptr->next = nextBlock->next;
  	block_ptr->size = block_ptr->size + META_SIZE + nextBlock->size;
  	nextBlock = block_ptr->next;
  }

  block_ptr->free = 1;
  block_ptr->magic = 0x55555555;

}

void *realloc(void *ptr, size_t size) {
  if (!ptr) {
    // NULL ptr. realloc should act like malloc.
    return malloc(size);
  }

  struct block_meta* block_ptr = get_block_ptr(ptr);
  if (block_ptr->size >= size) {
    // We have enough space. Could free some once we implement split.
    return ptr;
  }

  // Need to really realloc. Malloc new space and free old space.
  // Then copy old data to new space.
  void *new_ptr;
  new_ptr = malloc(size);
  if (!new_ptr) {
    return NULL; // TODO: set errno on failure.
  }
  memcpy(new_ptr, ptr, block_ptr->size);
  free(ptr);
  return new_ptr;
}

void *calloc(size_t nelem, size_t elsize) {
  size_t size = nelem * elsize; // TODO: check for overflow.
  void *ptr = malloc(size);
  memset(ptr, 0, size);
  return ptr;
}

int main(int argc, char *argv[]) {
    int i;
    for (i=0; i< 10000000; i++) {
        int* test = malloc(sizeof(int));
        *test = 16;
        
        free(test);
    }
    
    int* test = malloc(sizeof(int));
    test = realloc(test, 32);
    
}



