#include "malloc-header.c"

void malloc_stats1() {

  pthread_mutex_lock(&stats_lock);
  // if (stat_start == 0) {
  //   base_stats.next = stat_start = &base_stats;
  // }
  // else {
  //   int i = 0;
    Stat *p = stat_start;
    while (p != NULL) {
   
      printf("\nTotal size of arena allocated with sbrk/mmap:%d bytes",ArenaSize);
      printf("\nTotal number of bins: 5");
      printf("\nThread Arena %ld\n", (unsigned long)p->pid);
      printf("Used blocks: 8-bin:%d , 16-bin:%d, 64-bin:%d, 512-bin:%d, >512-bin:%d \n",
             p->used_blocks[0], p->used_blocks[1], p->used_blocks[2], p->used_blocks[3],p->used_blocks[4]);
      printf("Free blocks: 8-bin:%d , 16-bin:%d, 64-bin:%d, 512-bin:%d, >512-bin:%d \n",
             p->free_blocks[0], p->free_blocks[1], p->free_blocks[2], p->free_blocks[3],p->free_blocks[4]);
      printf("Total Allocation requests: %d\n", p->alloc_req);
      printf("Total free requests: %d\n\n", p->free_req);
      p = p->next;
    }
  // }
  pthread_mutex_unlock(&stats_lock);
}
 