#include <limits.h>
#include <pthread.h>
#include <unistd.h>

/**
Citations 
1) http://www.cs.kent.edu/~ruttan/sysprog/lectures/multi-thread/pthread_mutex_init.html
 **/
struct memory_block_list {
  size_t len;
  struct memory_block_list * next;
  struct memory_block_list * prev;
};

typedef struct memory_block_list mem_block_list;

extern mem_block_list * free_head = NULL;
extern pthread_mutex_t free_list_lock = PTHREAD_MUTEX_INITIALIZER;
extern pthread_mutex_t sbrk_lock = PTHREAD_MUTEX_INITIALIZER;

//Thread Safe malloc/free: locking version
void * ts_malloc_lock(size_t size);
void ts_free_lock(void * ptr);
