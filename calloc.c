#include "malloc-header.c"


void *calloc1(size_t nmemb, size_t size)
{
  //return NULL;
  // currStats->alloc_req += 1;

  if (nmemb == 0 || size == 0) return NULL;
  void *p = malloc1(nmemb * size);

  if (!p) return NULL;

  return memset(p, 0, nmemb * size);
}