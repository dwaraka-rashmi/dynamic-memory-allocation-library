#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "malloc.c"

int main(int argc, char **argv)
{
  
  size_t size = 12;
  void *mem = malloc1(size);
  printf("Successfully malloc'd %zu bytes at addr %p\n", size, mem);
  assert(mem != NULL);
  free1(mem);
  printf("Successfully free'd %zu bytes from addr %p\n", size, mem);

  size = 600;
  mem = malloc1(size);
  printf("Successfully malloc'd %zu bytes at addr %p\n", size, mem);
  assert(mem != NULL);
  free1(mem);
  printf("Successfully free'd %zu bytes from addr %p\n", size, mem);
  mem = malloc1(size);
  printf("Successfully malloc'd %zu bytes at addr %p\n", size, mem);
  assert(mem != NULL);
  free1(mem);
  printf("Successfully free'd %zu bytes from addr %p\n", size, mem);

  int *calloc_value = (int *)calloc1(1, sizeof(int));
  printf("Successfully allocated and initialized value of calloc int: %d from addr %p \n", *calloc_value,calloc_value);

  Header* realloc_test = (Header*)calloc_value - 1;
  printf("Initial size is %zu\n", (realloc_test->s.size));
  calloc_value = realloc1(calloc_value, 50);
  realloc_test = (Header*)calloc_value - 1;
  printf("Re-Initialized size to %zu\n", (realloc_test->s.size));
  free1(calloc_value);

  malloc_stats1();

  return 0;
}

// int main(int argc, char **argv)
// {
//   size_t size = 12;
//   void *mem = malloc1(size);
//   printf("Successfully malloc1'd %zu bytes at addr %p\n", size, mem);
//   assert(mem != NULL);
//   free11(mem);
//   printf("Successfully free11'd %zu bytes from addr %p\n", size, mem);
//   return 0;
// }

// int main(int argc, char **argv)
// {
  
//   size_t size = 12;
//   void *mem = malloc(size);
//   printf("Successfully malloc1'd %zu bytes at addr %p\n", size, mem);
//   assert(mem != NULL);
//   free1(mem);
//   printf("Successfully free11'd %zu bytes from addr %p\n", size, mem);

//   size = 60;
//   mem = malloc1(size);
//   printf("Successfully malloc1'd %zu bytes at addr %p\n", size, mem);
//   assert(mem != NULL);
//   free1(mem);
//   printf("Successfully free11'd %zu bytes from addr %p\n", size, mem);
//   mem = malloc1(size);
//   printf("Successfully malloc1'd %zu bytes at addr %p\n", size, mem);
//   assert(mem != NULL);
//   free1(mem);
//   printf("Successfully free11'd %zu bytes from addr %p\n", size, mem);

//   int *calloc1_value = (int *)calloc1(1, sizeof(int));
//   printf("Successfully allocated and initialized value of calloc1 int: %d from addr %p \n", *calloc1_value,calloc1_value);

//   //Header* realloc_test = (Header*)calloc1_value - 1;
//   //printf("Initial size is %zu\n", (realloc_test->s.size));
//   calloc1_value = realloc1(calloc1_value, 50);
//   //realloc_test = (Header*)calloc1_value - 1;
//   //printf("Re-Initialized size to %zu\n", (realloc_test->s.size));
//   free1(calloc1_value);

//   return 0;
// }