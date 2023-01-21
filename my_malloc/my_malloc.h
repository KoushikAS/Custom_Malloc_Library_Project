#include <unistd.h>

struct memory_block_list {
  size_t len;
  struct memory_block_list *next;
  struct memory_block_list *prev;
};

typedef struct memory_block_list mem_block_list; 

extern mem_block_list *free_head = NULL;
extern unsigned long heap_size =0;

void *ff_malloc(size_t size);
void ff_free(void * ptr);

unsigned long get_data_segment_size();
unsigned long get_data_segment_free_space_size();
