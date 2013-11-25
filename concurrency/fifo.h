#ifndef FIFO_H
#define FIFO_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <signal.h>
#include  "sem.h"
#define MYFIFO_BUFSIZ 4096

struct fifo{
	unsigned long buf[MYFIFO_BUFSIZ];
	int rStart, wStart;
	struct sem access, read, write;
};


/* Initialize the shared memory*/
void fifo_init( struct fifo *f);
/* Enqueue the data word d into the FIFO */
void fifo_wr(struct fifo *f, unsigned long d);
/* dequeue the next data word from fifo and return */
unsigned long fifo_rd( struct fifo *f);


#endif