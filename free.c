#include "malloc-header.c"

// void free(void *ptr)
// {
//   Header *hdr = ptr - sizeof(Header);
//   //printf("%s:%d free(%p): Freeing %zu bytes from %p\n",__FILE__, __LINE__, ptr, hdr->s.size, hdr);
//   munmap(hdr, hdr->s.size);
// }

void free1(void *ptr)
{
  // pthread_init();
  // printf("Entered free1 with %ld\n", (unsigned long)pthread_self());

  // // pthread_mutex_lock(&freelist_lock);
  // currStats->free_req += 1;
  // // pthread_mutex_unlock(&freelist_lock);
  Header *basePtr, *p;
  int index = 0;

  if(ptr == 0)
    return;

  //header of the block being freed, ptr is block freed
  basePtr = (Header*)ptr - 1;
  size_t size = basePtr->s.size;
  size = size - sizeof(Header);
  //printf("%d",size);

  if (size <= 8) index = 0;
  else if (size <= 16) index = 1;
  else if (size <= 64) index = 2;
  else if (size <= 512) index = 3;
  else index = 4;

  free_x_bin(ptr, index);

}

void free_x_bin(void *ptr, int index)
{

  Header *basePtr, *p;

  basePtr = (Header*)ptr - 1;
  size_t size = basePtr->s.size;

  
  int num_blocks = ceil(size/alloc_Size[index]);
 
  currStats->used_blocks[index] = currStats->used_blocks[index] - num_blocks;
  currStats->free_blocks[index] = currStats->free_blocks[index] + num_blocks; 

  currStats->free_req = currStats->free_req + 1;

  for (p = freep[index]->s.ptr; !(basePtr > p && basePtr < p->s.ptr); p = p->s.ptr) {
    if (p >= p->s.ptr && (basePtr > p || basePtr < p->s.ptr))
      break;
  }

  if (basePtr + basePtr->s.size == p->s.ptr) {
    basePtr->s.size += p->s.ptr->s.size;
    basePtr->s.ptr = p->s.ptr->s.ptr;
  }
  else basePtr->s.ptr = p->s.ptr;

  if (p + p->s.size == basePtr) {
    p->s.size += basePtr->s.size;
    p->s.ptr = basePtr->s.ptr;
  }
  else p->s.ptr = basePtr;

  freep[index] = p;



}
