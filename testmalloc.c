#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <pthread.h>
#include "malloc-headers.h"

pthread_t tid[100];
int counter;


void *testMalloc() {

    printf("Started testmalloc() thread with pid %ld\n", (unsigned long)pthread_self());
    size_t size = 12;
    void *mem = malloc1(size);
    printf("Successfully malloc'd %zu bytes at addr %p\n", size, mem);
    malloc_stats1();
    assert(mem != NULL);
    free1(mem);
    printf("Successfully free'd %zu bytes from addr %p\n", size, mem);
    malloc_stats1();

    printf("Trying calloc + for pid = %lu \n", pthread_self());
    int *test_int = (int *)calloc1(1, sizeof(int));
    printf("Initialized value of calloc int: %d from addr %p \n", *test_int, test_int);

    //free1(test_int);
    Header* size_ptr = (Header*)test_int - 1;
    printf("Initial size is %zu\n",size_ptr->s.size);

    malloc_stats1();

    test_int = (int *)realloc1(test_int, 45);
    malloc_stats1();
    size_ptr = (Header*)test_int - 1;
    printf("Reinitialized size to %zu\n", (size_ptr->s.size - 1) + sizeof(Header));
    free1(test_int);

    malloc_stats1();
    printf("\nComplete");

    return 0;
}

void startThreads() {
    int i = 0, num_of_threads = 1, err;

    while (i < num_of_threads)
    {
        printf("Entered while loop with thread %d\n", i);
        printf("\n before create \n");
        err = pthread_create(&(tid[i]), NULL, &testMalloc, NULL);
        printf("\n after create \n");
        if (err != 0)
            printf("\ncan't create thread :[%s]", strerror(err));
        else
            printf("Created thread with pid %lu\n", pthread_self());
        i++;
    }

    i = 0;
    while (i < num_of_threads) {
        pthread_join(tid[i], NULL);
        i++;
    }

    return;
}


int main(void)
{

    pid_t  pid;

    // pid = fork();
    // if (pid == 0)
    //     testMalloc();
    // else
    //     testMalloc();
    testMalloc();
    startThreads();

    return 0;
}