#include <assert.h>
#include <string.h>
#include <stdint.h>
#include "malloc.h"
#include <stdio.h>
#include <stdlib.h>

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

static void *global_base = NULL;

static void *top = 0;

void *mySbrk(int increment) {
    if (increment < 0) {
        return ((void*) -1);
    }
    
    if (increment == 0) {
        return (void*)top;
    }
    
	void *result = top;
    top += increment;
    
    ssize_t ret;
    __asm__ volatile
    (
     "syscall"
     : "=a" (ret)
     : "0"(12), "D"(top)
     : "cc", "rcx", "r11"
    ); //may also need "memory" clobber
	
	return result;
}

static struct block_meta *find_free_block(struct block_meta **last, size_t size) {
  struct block_meta *current = global_base;
  while (current && !(current->free && current->size >= size)) {
    *last = current;
    current = current->next;
  }
  return current;
}

static struct block_meta *request_space(struct block_meta* last, size_t size) {
  struct block_meta *block;
  block = mySbrk(0);
  void *request = mySbrk(size + META_SIZE);
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

void *myMalloc(size_t size) {
  struct block_meta *block;
  // TODO: align size?

  if (size <= 0) {
    return NULL;
  }

  if (!global_base) { // First call.
      
      //get the initial end of segment address
      __asm__ volatile ( "syscall"
               : "=a" (top)
               : "0"(12), "D"(-1)
               : "cc", "rcx", "r11"
               );
      
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

static struct block_meta *get_block_ptr(void *ptr) {
  return (struct block_meta*)ptr - 1;
}

void myFree(void *ptr) {
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

void *myRealloc(void *ptr, size_t size) {
  if (!ptr) {
    // NULL ptr. realloc should act like malloc.
    return myMalloc(size);
  }

  struct block_meta* block_ptr = get_block_ptr(ptr);
  if (block_ptr->size >= size) {
    // We have enough space. Could free some once we implement split.
    return ptr;
  }

  // Need to really realloc. Malloc new space and free old space.
  // Then copy old data to new space.
  void *new_ptr;
  new_ptr = myMalloc(size);
  if (!new_ptr) {
    return NULL; // TODO: set errno on failure.
  }
  memcpy(new_ptr, ptr, block_ptr->size);
  myFree(ptr);
  return new_ptr;
}

void *myCalloc(size_t nelem, size_t elsize) {
  size_t size = nelem * elsize; // TODO: check for overflow.
  void *ptr = myMalloc(size);
  memset(ptr, 0, size);
  return ptr;
}

int main(int argc, char *argv[]) {
    printf("entered main\n");
    int i;
    for (i=0; i< 10; i++) {
        int* test = myMalloc(sizeof(int));
        *test = 16;
        
        myFree(test);
    }
    
    int* test = myMalloc(sizeof(int));
    test = myRealloc(test, 32);
}



