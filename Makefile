#Sample Makefile for Malloc
CC=gcc
CFLAGS=-g -O0 -fPIC -pthread -lm

all:	check

default: build

build:
	gcc -o savan-test savan_test.c -pthread -lrt -g -Wall -lm
	./savan-test

t_test1:
	gcc -g t_test1.c -o t_test1 -pthread -lm

testmalloc:
	gcc -g testmalloc.c -o testmalloc -pthread -lm

clean:
	rm -rf *.o *.so t_test1 testmalloc test1 result.out savan-test
	clear

calloc.so: calloc.o
	$(CC) $(CFLAGS) -shared -Wl,--unresolved-symbols=ignore-all $< -o $@

free.so: free.o
	$(CC) $(CFLAGS) -shared -Wl,--unresolved-symbols=ignore-all $< -o $@

realloc.so: realloc.o
	$(CC) $(CFLAGS) -shared -Wl,--unresolved-symbols=ignore-all $< -o $@

memstats.so: memstats.o
	$(CC) $(CFLAGS) -shared -Wl,--unresolved-symbols=ignore-all $< -o $@

malloc-header.so: malloc-header.o
	$(CC) $(CFLAGS) -shared -Wl,--unresolved-symbols=ignore-all $< -o $@

libmalloc.so: malloc.o 
	$(CC) $(CFLAGS) -shared -Wl,--unresolved-symbols=ignore-all $< -o $@
	

# For every XYZ.c file, generate XYZ.o.

%.o: %.c
	$(CC) $(CFLAGS) $< -c -o $@

# check:	libmalloc.so testmalloc calloc.so realloc.so free.so memstats.so
# 	LD_PRELOAD=`pwd`/libmalloc.so `pwd`/calloc.so `pwd`/realloc.so `pwd`/free.so `pwd`/memstats.so ./testmalloc

check:	libmalloc.so testmalloc 
	LD_PRELOAD=`pwd`/libmalloc.so ./testmalloc

test:	libmalloc.so t_test1
	LD_PRELOAD=`pwd`/libmalloc.so ./t_test1

test1:	libmalloc.so savan_test
	LD_PRELOAD=`pwd`/libmalloc.so ./savan_test


dist:
	dir=`basename $$PWD`; cd ..; tar cvf $$dir.tar ./$$dir; gzip $$dir.tar
