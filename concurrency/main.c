#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "sem.h"
#include "fifo.h"

int main(int argc, char**argv){

	printf("Initializing Semaphore...\n");
	struct sem testSem;
	sem_init(&testSem,15);
	printf("Semaphore val: %d\n",testSem.count);
	// printf("Testing: %d\n",sem_try(&testSem));
	
	printf("Initializing Fifo...\n");
	struct fifo testfifo;
	fifo_init(&testfifo);
	return 0;
}