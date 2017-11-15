#include "malloc-header.c"

void *realloc1(void *ptr, size_t size)
{
  // Allocate new memory (if needed) and copy the bits from old location to new.
  //return NULL;
  if(ptr == NULL)
    return malloc1(size);

  Header *basePtr, *p;
  basePtr = (Header*)ptr - 1;
  // size_t allocSize =  basePtr->s.size - 1;
  size_t oldSize =  basePtr->s.size;

  if (size <= oldSize) {
    // basePtr->s.size = size;
    return (void *)ptr;
  }

  void *mem = malloc1(size);
  memcpy(mem , ptr , oldSize);
  free1(ptr);
  return (void *)mem;

}