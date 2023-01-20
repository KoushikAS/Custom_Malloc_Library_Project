#include "my_malloc.h"
#include <stdio.h>
#include <stdlib.h>

void *ff_malloc(size_t size){
  if(head == NULL){ //Base Condition
     void *allocated_mem = sbrk(sizeof(mem_block_list) + size);
     mem_block_list *metadata = (mem_block_list *)allocated_mem;
     metadata->len = size;
     metadata->next = NULL;
     metadata->prev = NULL;
    
     return allocated_mem + sizeof(mem_block_list);
  }
  return NULL;
}

void ff_free(void * ptr){
  
  mem_block_list *metadata = (mem_block_list *)(ptr - sizeof(mem_block_list));
  
  if(head == NULL) { //Adding first element in the list
    head = metadata;
    return;
  }

  
  if(head > metadata){ //Adding to the front (i.e. Head)
    metadata->next = head;
    head->previous = metadata;
    head = metadata;
    return; 
  }
  
  memory_block_list *curr = head;
  while(curr->next != NULL && curr->next < metadata){
    curr = curr->next;
  }

  metadata->next = curr->next;
  metadata->prev = curr;
  if(curr->next != NULL){ //When we are adding to the end of the list.
    curr->next->prev = metadata;
  }
  curr->next = metadata;
  
  printf("something*");

  
  
}
  
  /**

  
  size_t size = *metadata;
  printf("Size_t %zu\n", size);

  memory_block_list block;
  block.start_address = ptr - sizeof(size_t);
  block.len = sizeof(size_t) + size;
  block.end_address = ptr + block.len;


  if(head == NULL) { //Adding first element in the list
    block.previous = NULL;
    block.next = NULL;
    head = &block;
    return;
  }

  if(head->start_address > block.start_address){ //Adding to the front (i.e. Head)
    block.previous = NULL;
    block.next = head;
    head->previous = &block;
    head = &block;
    return; 
  }
  
  memory_block_list *curr = head;
  while(curr->next != NULL && curr->next->start_address < block.start_address){
    curr = curr->next;
  }

  block.next = curr->next;
  block.previous = curr;
  if(curr->next != NULL){ //When we are adding to the end of the list.
    curr->next->previous = &block;
  }
  curr->next = &block;
  
  printf("something*");
}
  /** 
  memory_block_list **curr = &head;
  while(*curr != NULL && ((*curr)->start_address + (*curr)->len) < block.start_address){
   curr = &((*curr)->next);
  }
  if(*curr == NULL){
   block.next = NULL;
  }else{
   block.next = (*curr)->next;
  }
    
  *curr = &block;
  **/
