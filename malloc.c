#include "malloc-header.c"


static Header* morecore(size_t allocSize, size_t size, int index)
{
  char *p;
  Header *hp;
  long pageSize = sysconf(_SC_PAGESIZE);

  //For memory requests >512 bytes
  if (index == 4) {

    void *ret = mmap(0, size + sizeof(Header), PROT_READ | PROT_WRITE,
                     MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    assert(ret != MAP_FAILED);

    Header *hdr = (Header*) ret;
    hdr->s.size = size;

    hdr->s.ptr = freep[index]->s.ptr;
    freep[index]->s.ptr = hdr;
    ArenaSize += size;
    currStats->size = ArenaSize;
    currStats->free_blocks[index] += size;
    return freep[index];

  }

  if ((allocSize * sizeof(Header)) < pageSize)
    allocSize = pageSize;

  //sbrk system call is not thread safe
  pthread_mutex_lock(&freelist_lock);
  p = sbrk(allocSize * sizeof(Header));
  pthread_mutex_unlock(&freelist_lock);

  if (p == (char*) - 1) {
    errno = ENOMEM;
    return NULL;
  }

  hp = (Header*)p;
  hp->s.size = allocSize;

  ArenaSize += allocSize*(sizeof(Header));
  currStats->size = ArenaSize;

  currStats->free_blocks[index] += allocSize;

  hp->s.ptr = freep[index]->s.ptr;
  freep[index]->s.ptr = hp;

  return freep[index];

}

void *malloc_x_bin(size_t allocSize, size_t size, int  index) {

  Header *p, *prevp;

  if ((prevp = freep[index]) == 0) {
    base[index].s.ptr = freep[index] = prevp = &base[index];
    base[index].s.size = 0;
  }

  for (p = base[index].s.ptr; ; prevp = p, p = p->s.ptr) {

    if (p->s.size >= allocSize) {

      if (p->s.size == size)
        prevp->s.ptr = p->s.ptr;
      else {
        p->s.size -= allocSize;
        p += p->s.size;
        if(index==0){
          p->s.size = allocSize*alloc_Size[0];
        }
        else p->s.size = allocSize*sizeof(Header);
      }

      freep[index] = prevp;
      return (void*)(p + 1);
    }

    if (p == freep[index])
      if ((p = morecore(allocSize, size, index)) == 0)
        return NULL;
  }

}

void *check_pid(int pid) {
  Stat *p = stat_start;
  while (p != NULL) {
    if (p->pid == pid)
      return p;
    else
      p = p->next;
  }
  return NULL;
}

void *malloc1(size_t size)
{
  // TODO: Validate size.
  //size_t allocSize = size + sizeof(Header);
  //Rounding of the requested size to multiple of header size
  size_t allocSize = (size + sizeof(Header) - 1) / sizeof(Header) + 1;

  int index = 0;

  pthread_mutex_lock(&stats_lock);
  if (stat_start == 0) {
    base_stats.next = NULL;
    stat_start = &base_stats;
    base_stats.pid = pthread_self();
    base_stats.alloc_req = 1;
    base_stats.free_req = 0;
    currStats = &base_stats;
    stat_start = currStats;
  }
  else {
    pthread_t pid = pthread_self();
    if (check_pid(pid) == NULL) {
      Stat *p = stat_start;
      while (p->next != NULL)
        p = p->next;
      p->next = currStats;
      currStats->next = NULL;

      currStats->pid = pid;
      currStats->alloc_req += 1;

    }
    else {
      currStats->alloc_req += 1;
    }
  }
    pthread_mutex_unlock(&stats_lock);

  if (size <= 8){ 
    index = 0;
    allocSize +=1;
  }
  else if (size <= 16) index = 1;
  else if (size <= 64) index = 2;
  else if (size <= 512) index = 3;
  else index = 4;

  void * ret = malloc_x_bin(allocSize, size, index);

  int num_blocks = 0;
  //Since 8-bin size is not inmultipe of header size
  if(index==0){
    num_blocks = ceil((allocSize*alloc_Size[index])/alloc_Size[0]);
  }
  else num_blocks = ceil((allocSize*sizeof(Header))/alloc_Size[index]);
  currStats->used_blocks[index] += num_blocks;
  currStats->free_blocks[index] -= num_blocks;

  return ret;

}

