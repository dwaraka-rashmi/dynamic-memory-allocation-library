#ifndef MALLOC_DATA
#define MALLOC_DATA


#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <pthread.h>
#include <math.h>

/* Align to the double value*/
typedef double Align;

union header {
  struct {
    union header *ptr;
    //union header *prev;  //to make it doubly linkedlist
    size_t size;
  } s;
  Align x;
};

typedef union header Header;

//free list - separate for 8,16,64,512,GT_512 bytes
static __thread  Header base[5];
static __thread  Header *freep[5];
static int alloc_Size[5] = {8, 16, 64, 512, 512};
static int ArenaSize;


//malloc stats - shared across the threads
typedef struct Stats {
  struct Stats* next;
  pthread_t pid;
  int used_blocks[5];
  int free_blocks[5];
  int alloc_req;
  int free_req;
  int size;
} Stat;

static Stat base_stats;
static Stat *stat_start;
static __thread Stat *currStats = &(Stat) {.next = NULL,
                                           .pid = 0,
                                           .used_blocks = {0},
                                           .free_blocks = {0},
                                           .alloc_req = 0,
                                           .free_req = 0};


//Thread initialization
pthread_mutex_t stats_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t freelist_lock = PTHREAD_MUTEX_INITIALIZER;

static void pre_fork(void) {
  pthread_mutex_lock(&stats_lock);
  pthread_mutex_lock(&freelist_lock);
}

static void post_fork_parent(void) { 
  pthread_mutex_unlock(&stats_lock);
  pthread_mutex_unlock(&freelist_lock);
}

static void post_fork_child(void)  { 
  pthread_mutex_init(&stats_lock, NULL);
  pthread_mutex_init(&freelist_lock, NULL);
}

__attribute__((constructor))
static void pthread_init(void) {
 int rc = pthread_atfork(&pre_fork, &post_fork_parent, &post_fork_child);
}

void malloc_stats1();
void free_x_bin(void *ptr, int index);
void free1(void *ptr);
static Header* morecore(size_t allocSize, size_t size, int index);
void *malloc_x_bin(size_t allocSize, size_t size, int  index);
void *check_pid(int pid);
void *malloc1(size_t size);
void *realloc1(void *ptr, size_t size);
void *calloc1(size_t nmemb, size_t size);

#endif