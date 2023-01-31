#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "my_malloc.h"

#ifdef FF
#define MALLOC(sz) ff_malloc(sz)
#define FREE(p) ff_free(p)
#endif
#ifdef BF
#define MALLOC(sz) ts_malloc_lock(sz)
#define FREE(p) ts_free_lock(p)
#endif

int main(int argc, char * argv[]) {
  const unsigned NUM_ITEMS = 10;
  int i;
  int size;
  int sum = 0;
  int expected_sum = 0;
  int * array[NUM_ITEMS];

  size = 4;
  expected_sum += size * size;
  array[0] = (int *)MALLOC(size * sizeof(int));
  printf("Sbrk %p\n", sbrk(0));
  printf("Sbrk %p\n", sbrk(0));

  assert(array[0] + size <= (int *)sbrk(0));
  printf("Sbrk %p\n", sbrk(0));

  for (i = 0; i < size; i++) {
    printf("i = %d Sbrk %p\n", i, sbrk(0));

    array[0][i] = size;
  }  //for i
  printf("Sbrk %p\n", sbrk(0));

  for (i = 0; i < size; i++) {
    sum += array[0][i];
  }  //for i
  printf("Sbrk %p\n", sbrk(0));

  printf("I=0\n");
  size = 16;
  expected_sum += size * size;
  printf("Sbrk %p\n", sbrk(0));

  array[1] = (int *)MALLOC(size * sizeof(int));
  assert(array[1] + size <= (int *)sbrk(0));
  //  assert((void *)array[0] + (4 * sizeof(int)) + sizeof(mem_block_list) ==     (void *)array[1]);
  for (i = 0; i < size; i++) {
    array[1][i] = size;
  }  //for i
  for (i = 0; i < size; i++) {
    sum += array[1][i];
  }  //for i

  printf("I=1\n");

  size = 8;
  expected_sum += size * size;
  array[2] = (int *)MALLOC(size * sizeof(int));
  assert(array[2] + size <= (int *)sbrk(0));
  for (i = 0; i < size; i++) {
    array[2][i] = size;
  }  //for i
  for (i = 0; i < size; i++) {
    sum += array[2][i];
  }  //for i

  printf("I=2\n");

  size = 32;
  expected_sum += size * size;
  array[3] = (int *)MALLOC(size * sizeof(int));
  assert(array[3] + size <= (int *)sbrk(0));
  for (i = 0; i < size; i++) {
    array[3][i] = size;
  }  //for i
  for (i = 0; i < size; i++) {
    sum += array[3][i];
  }  //for i

  printf("I=3\n");

  FREE(array[0]);
  FREE(array[1]);
  FREE(array[2]);

  printf("FREE 0 1 2\n");

  size = 7;
  expected_sum += size * size;
  array[4] = (int *)MALLOC(size * sizeof(int));
  assert(array[4] + size <= (int *)sbrk(0));
  for (i = 0; i < size; i++) {
    array[4][i] = size;
  }  //for i
  for (i = 0; i < size; i++) {
    sum += array[4][i];
  }  //for i

  printf("I=4\n");

  size = 256;
  expected_sum += size * size;
  array[5] = (int *)MALLOC(size * sizeof(int));
  assert(array[5] + size <= (int *)sbrk(0));
  for (i = 0; i < size; i++) {
    array[5][i] = size;
  }  //for i
  for (i = 0; i < size; i++) {
    sum += array[5][i];
  }  //for i

  printf("I=5\n");

  FREE(array[5]);
  //FREE(array[1]);
  FREE(array[3]);

  printf("FREE 5 3\n");

  size = 23;
  expected_sum += size * size;
  array[6] = (int *)MALLOC(size * sizeof(int));
  assert(array[6] + size <= (int *)sbrk(0));
  for (i = 0; i < size; i++) {
    array[6][i] = size;
  }  //for i
  for (i = 0; i < size; i++) {
    sum += array[6][i];
  }  //for i

  printf("I=6\n");

  size = 4;
  expected_sum += size * size;
  array[7] = (int *)MALLOC(size * sizeof(int));
  assert(array[7] + size <= (int *)sbrk(0));
  for (i = 0; i < size; i++) {
    array[7][i] = size;
  }  //for i
  for (i = 0; i < size; i++) {
    sum += array[7][i];
  }  //for i

  printf("I=7\n");

  FREE(array[4]);

  size = 10;
  expected_sum += size * size;
  array[8] = (int *)MALLOC(size * sizeof(int));
  assert(array[8] + size <= (int *)sbrk(0));
  for (i = 0; i < size; i++) {
    array[8][i] = size;
  }  //for i
  for (i = 0; i < size; i++) {
    sum += array[8][i];
  }  //for i

  printf("I=8\n");

  size = 32;
  expected_sum += size * size;
  array[9] = (int *)MALLOC(size * sizeof(int));
  assert(array[9] + size <= (int *)sbrk(0));
  for (i = 0; i < size; i++) {
    array[9][i] = size;
  }  //for i
  for (i = 0; i < size; i++) {
    sum += array[9][i];
  }  //for i

  printf("I=9\n");

  FREE(array[6]);
  FREE(array[7]);
  FREE(array[8]);
  FREE(array[9]);

  if (sum == expected_sum) {
    printf("Calculated expected value of %d\n", sum);
    printf("Test passed\n");
  }
  else {
    printf("Expected sum=%d but calculated %d\n", expected_sum, sum);
    printf("Test failed\n");
  }  //else

  return 0;
}
