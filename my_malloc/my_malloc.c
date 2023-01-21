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

void addMetadataToList(mem_block_list **head,  mem_block_list **metadata){

    if(*head == NULL) { //Adding first element in the list
       *head = *metadata;
       return;
     }
  
     if(*head > *metadata){ //Adding to the front (i.e. Head)
       (*metadata)->next = *head;
       (*head)->prev = *metadata;
       *head = *metadata;
       return; 
     }
  
     mem_block_list *curr = *head;
     while(curr->next != NULL && curr->next < *metadata){
       curr = curr->next;
     }

     (*metadata)->next = curr->next;
     (*metadata)->prev = curr;
     if(curr->next != NULL){ //To check if it is not the end of list.
       curr->next->prev = *metadata;
     }
     curr->next = *metadata;
}

void *expandHeap(size_t size){
     void *allocated_mem = sbrk(sizeof(mem_block_list) + size);
     mem_block_list *metadata = createNewMetaDataNode(allocated_mem, size);
     addMetadataToList(&alloc_head, &metadata);     
     return (void *)allocated_mem + sizeof(mem_block_list);
}

void *allocateFromFreeSpace(mem_block_list *curr){
	mem_block_list *prev_free = curr->prev;
	mem_block_list *next_free = curr->next;

	//Make sure the Free list is proper
	if(prev_free == NULL){ //1st element
	  free_head = next_free;
	} else {
	  prev_free->next = next_free;
	}

	if(next_free != NULL){
	  next_free->prev = prev_free;
	}

	//Allocating the new memory	
	curr->next = NULL;
	curr->prev = NULL;

	addMetadataToList(&alloc_head, &curr);
	  
	return (void *)curr + sizeof(mem_block_list);  
}

void *ff_malloc(size_t size){
  
    mem_block_list *curr = free_head;
    
    while(curr != NULL){
      if(size <= curr->len){
	size_t extra_space = curr->len - size;

	
	//Trying to Optimize by allocating exactly what user requested.
		
	if(extra_space > sizeof(mem_block_list)){
	  mem_block_list *metadata = createNewMetaDataNode((void *)curr + sizeof(mem_block_list) + size, extra_space - sizeof(mem_block_list));
	  metadata->prev = curr;
	  metadata->next = curr->next;
	  if(curr->next != NULL){
	    curr->next->prev = metadata;
	  }
	  curr->next = metadata;
	  curr->len = size;
	}
	
	return allocateFromFreeSpace(curr);
      }
      else{
	curr = curr->next;
      }
    }
    
    return expandHeap(size);
}


void coalesce(){ 
  mem_block_list *curr = free_head;
  while(curr != NULL){
    if((curr->next != NULL) && ((void *)curr + curr->len + sizeof(mem_block_list) == curr->next)){
	curr->len = curr->len + curr->next->len + sizeof(mem_block_list); //Also adding space of mem_block_list
	mem_block_list *next = curr->next->next;
	if(next != NULL){
	  next->prev = curr;
	}
	curr->next = next;
    } else{
      curr = curr->next;
    }
  }
}

void ff_free(void * ptr){
  mem_block_list *metadata = (mem_block_list *)(ptr - sizeof(mem_block_list));

  mem_block_list *prev_alloc = metadata->prev;
  mem_block_list *next_alloc = metadata->next;

  //Make sure the Free list is proper
  if(prev_alloc == NULL){ //1st element
    alloc_head = next_alloc;
  } else {
    prev_alloc->next = next_alloc;
  }

  if(next_alloc != NULL){
    next_alloc->prev = prev_alloc;
  }

  metadata->prev = NULL;
  metadata->next = NULL;
  
  addMetadataToList(&free_head, &metadata);
  coalesce();
}

unsigned long get_segment_size(mem_block_list *head){
  unsigned long size =0;

  mem_block_list *curr = head;

  while(curr != NULL){
    size = size + sizeof(mem_block_list) + curr->len;
    curr = curr->next;
  }

  return size;
}

unsigned long get_data_segment_size(){  
  return get_segment_size(alloc_head) + get_segment_size(free_head);
}


unsigned long get_data_segment_free_space_size(){
  return get_segment_size(free_head);
}
