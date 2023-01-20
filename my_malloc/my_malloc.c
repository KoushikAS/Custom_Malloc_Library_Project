#include "my_malloc.h"
#include <stdio.h>
#include <stdlib.h>


void *expandHeap(size_t size){
     void *allocated_mem = sbrk(sizeof(mem_block_list) + size);
     mem_block_list *metadata = (mem_block_list *)allocated_mem;
     metadata->len = size;
     metadata->next = NULL;
     metadata->prev = NULL;
    
     return allocated_mem + sizeof(mem_block_list);
}

void *allocateFromFreeSpace(mem_block_list *curr, size_t size){
	mem_block_list *prev = curr->prev;
	mem_block_list *next = curr->next;

	if(prev == NULL){ //1st element
	  head = next;
	} else {
	  prev->next = next;
	}

	if(next != NULL){
	  next->prev = prev;
	}

	curr->next = NULL;
	curr->prev = NULL;
	  
	return curr + sizeof(mem_block_list);  
}


void *ff_malloc(size_t size){
  
    mem_block_list *curr = head;

    while(curr != NULL){
      if(size <= curr->len){
	return allocateFromFreeSpace(curr, size);
      }
      else{
	curr = curr->next;
      }
    }

    return expandHeap(size);
}

void addNewNode(void *ptr){
  mem_block_list *metadata = (mem_block_list *)(ptr - sizeof(mem_block_list));
  
  if(head == NULL) { //Adding first element in the list
    head = metadata;
    return;
  }
  
  if(head > metadata){ //Adding to the front (i.e. Head)
    metadata->next = head;
    head->prev = metadata;
    head = metadata;
    return; 
  }
  
  mem_block_list *curr = head;
  while(curr->next != NULL && curr->next < metadata){
    curr = curr->next;
  }

  metadata->next = curr->next;
  metadata->prev = curr;
  if(curr->next != NULL){ //To check if it is not the end of list.
    curr->next->prev = metadata;
  }
  curr->next = metadata;
}

void coalesce(){ 
  mem_block_list *curr = head;
  while(curr != NULL){
    if((curr->next != NULL) && (curr + curr->len + sizeof(mem_block_list) == curr->next)){
	curr->len = curr->len + curr->next->len + sizeof(mem_block_list); //Also adding space of mem_block_list 
	curr->next = curr->next->next;
      }
      curr = curr->next;
  }
}

void ff_free(void * ptr){
  addNewNode(ptr);
  coalesce();
}
