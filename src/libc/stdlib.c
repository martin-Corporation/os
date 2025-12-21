#include <stdio.h>
#include <stdlib.h>

// TODO: Store current_break somewhere else
uintptr_t current_break;

void *sbrk(intptr_t incr) {
  uintptr_t old_break = current_break;
  current_break += incr;
  return (void *)old_break;
}

// Source - https://stackoverflow.com/a
// Posted by Sylvain Defresne, modified by community. See post 'Timeline' for
// change history Retrieved 2025-12-20, License - CC BY-SA 3.0

typedef struct free_block {
  size_t size;
  struct free_block *next;
} free_block;

static free_block free_block_list_head = {0, 0};
static const size_t overhead = sizeof(size_t);
static const size_t align_to = 16;

void *malloc(size_t size) {
  size = (size + sizeof(size_t) + (align_to - 1)) & ~(align_to - 1);
  free_block *block = free_block_list_head.next;
  free_block **head = &(free_block_list_head.next);
  while (block != 0) {
    if (block->size >= size) {
      *head = block->next;
      return ((char *)block) + sizeof(size_t);
    }
    head = &(block->next);
    block = block->next;
  }

  block = (free_block *)sbrk(size);
  block->size = size;

  return ((char *)block) + sizeof(size_t);
}

void free(void *ptr) {
  free_block *block = (free_block *)(((char *)ptr) - sizeof(size_t));
  block->next = free_block_list_head.next;
  free_block_list_head.next = block;
}
