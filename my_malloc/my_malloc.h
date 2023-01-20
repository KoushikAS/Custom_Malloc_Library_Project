#include <unistd.h>

struct memory_block_list {
  void *addr;
  int len;
  struct memory_block_list *next;
  struct memory_block_list *prev;
};

typedef struct memory_block_list mem_block_list; 

extern mem_block_list *head = NULL;

void *ff_malloc(size_t size);
void ff_free(void * ptr);

