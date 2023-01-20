#include "my_malloc.h"
#include <stdio.h>
#include <stdlib.h>

mem_block_list *createNewMetaDataNode(void * mem, size_t size){
     mem_block_list *metadata = (mem_block_list *)mem;
     metadata->len = size;
     metadata->next = NULL;
     metadata->prev = NULL;
     return metadata;     
}

void *expandHeap(size_t size){
     void *allocated_mem = sbrk(sizeof(mem_block_list) + size);
     createNewMetaDataNode(allocated_mem, size);    
     return (void *)allocated_mem + sizeof(mem_block_list);
}

void *allocateFromFreeSpace(mem_block_list *curr){
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
	  
	return (void *)curr + sizeof(mem_block_list);  
}

void *ff_malloc(size_t size){
  
    mem_block_list *curr = head;
    
    while(curr != NULL){
      if(size <= curr->len){
	size_t extra_space = curr->len - size;

	/**
	//Trying to Optimize by allocating exactly what user requested.
		
	if(extra_space > sizeof(mem_block_list)){
	  mem_block_list *metadata = createNewMetaDataNode(curr + sizeof(mem_block_list) + size, extra_space - sizeof(mem_block_list));
	  metadata->prev = curr;
	  metadata->next = curr->next;
	  curr->next = metadata;
	  curr->len = size;
	}
	**/
	return allocateFromFreeSpace(curr);
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
    } else{
      curr = curr->next;
    }
  }
}

void ff_free(void * ptr){
  addNewNode(ptr);
  coalesce();
}
