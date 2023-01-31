#include "my_malloc.h"

mem_block_list * createNewMetaDataNode(void * mem, size_t size) {
  mem_block_list * metadata = (mem_block_list *)mem;
  metadata->len = size;
  metadata->next = NULL;
  metadata->prev = NULL;
  return metadata;
}

// Read lock is required
mem_block_list * search_list(size_t size) {
  mem_block_list * curr = free_head;
  mem_block_list * bst_fit = NULL;
  size_t bst_fit_size = UINT_MAX;

  while (curr != NULL) {
    if (size <= curr->len) {
      size_t extra_space = curr->len - size;
      if (extra_space < bst_fit_size) {
        bst_fit = curr;
        bst_fit_size = extra_space;
        if (extra_space == 0) {  //Found the Best fit in the list no need to go further
          break;
        }
      }
    }

    curr = curr->next;
  }

  return bst_fit;
}

void * allocateFromFreeSpace(mem_block_list * curr) {
  mem_block_list * prev_free = curr->prev;
  mem_block_list * next_free = curr->next;

  // Make sure the Free list is proper
  if (prev_free == NULL) {  // 1st element
    free_head = next_free;
  }
  else {
    prev_free->next = next_free;
  }

  if (next_free != NULL) {
    next_free->prev = prev_free;
  }

  // Allocating the new memory
  curr->next = NULL;
  curr->prev = NULL;
  return (void *)curr + sizeof(mem_block_list);
}

void split_extra_space(mem_block_list * curr, size_t size) {
  size_t extra_space = curr->len - size;

  // Trying to Optimize by allocating exactly what user requested.
  if (extra_space > sizeof(mem_block_list)) {
    mem_block_list * metadata =
        createNewMetaDataNode((void *)curr + sizeof(mem_block_list) + size,
                              extra_space - sizeof(mem_block_list));
    metadata->prev = curr;
    metadata->next = curr->next;
    if (curr->next != NULL) {
      curr->next->prev = metadata;
    }
    curr->next = metadata;
    curr->len = size;
  }
}

void * allocate_from_freelist(size_t size) {
  /** critical section **/
  void * bst_fit = search_list(size);

  if (bst_fit == NULL) {
    return NULL;
  }
  else {
    /** Write Critical section **/
    split_extra_space(bst_fit, size);
    return allocateFromFreeSpace(bst_fit);
    /** End of Write Critical Section **/
  }
  /** End of critical Section **/
}

void * expandHeap(size_t size) {
  void * allocated_mem = sbrk(sizeof(mem_block_list) +
                              size);  //Critical section since sbrk is not thread safe

  mem_block_list * metadata = createNewMetaDataNode(allocated_mem, size);
  return (void *)allocated_mem + sizeof(mem_block_list);
}

//Thread Safe malloc/free: locking version
void * ts_malloc_lock(size_t size) {
  pthread_mutex_lock(&free_list_lock);
  void * allocated_space = NULL;
  void * bst_fit_from_list = allocate_from_freelist(size);

  if (bst_fit_from_list != NULL) {
    allocated_space = bst_fit_from_list;
  }
  else {
    allocated_space = expandHeap(size);
  }
  pthread_mutex_unlock(&free_list_lock);
  return allocated_space;
}

void addMetadataToList(mem_block_list ** head, mem_block_list ** metadata) {
  if (*head == NULL) {  // Adding first element in the list
    *head = *metadata;
    return;
  }

  if (*head > *metadata) {  // Adding to the front (i.e. Head)
    (*metadata)->next = *head;
    (*head)->prev = *metadata;
    *head = *metadata;
    return;
  }

  mem_block_list * curr = *head;
  while (curr->next != NULL && curr->next < *metadata) {
    curr = curr->next;
  }

  (*metadata)->next = curr->next;
  (*metadata)->prev = curr;
  if (curr->next != NULL) {  // To check if it is not the end of list.
    curr->next->prev = *metadata;
  }
  curr->next = *metadata;
}

void coalesce(mem_block_list * curr) {
  while ((curr->next != NULL) &&
         ((void *)curr + curr->len + sizeof(mem_block_list) == curr->next)) {
    curr->len = curr->len + curr->next->len +
                sizeof(mem_block_list);  // Also adding space of mem_block_list

    mem_block_list * coalaced_block = curr->next;
    mem_block_list * next = curr->next->next;

    coalaced_block->next = NULL;
    coalaced_block->prev = NULL;
    coalaced_block->len = 0;

    if (next != NULL) {
      next->prev = curr;
    }
    curr->next = next;
  }
}

void ts_free_lock(void * ptr) {
  pthread_mutex_lock(&free_list_lock);
  mem_block_list * metadata = (mem_block_list *)(ptr - sizeof(mem_block_list));
  addMetadataToList(&free_head, &metadata);

  //Trying to Coalesce previous term
  if (metadata->prev != NULL) {
    coalesce(metadata->prev);
  }

  //Trying to Coalesce next terms
  coalesce(metadata);
  pthread_mutex_unlock(&free_list_lock);
}
