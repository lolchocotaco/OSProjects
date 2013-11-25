#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "sem.h"

int main(int argc, char**argv){

	printf("Initializing Semaphore...\n");
	struct sem testSem;
	sem_int(&testSem,15);
	printf("Semaphore val: %d\n",testSem.count);
	// printf("Testing: %d\n",sem_try(&testSem));
	return 0;
}