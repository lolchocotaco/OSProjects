#ifndef SEM_H
#define SEM_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <signal.h>
#define N_PROC 64

int myprocnum;

/* Semaphore Structure */
struct sem {
	unsigned int count;
	pid_t waiting[N_PROC];
	volatile char lock;
};


/* Assembly Code*/
int tas(volatile char *lock);
/* Initialize the sempahore *s with the initial count */
void sem_int( struct sem *s, int count);
/* Attept to atomically decrement the semaphore */
int sem_try(struct sem *s);
/* Perfom operation while blocking until successful*/
void sem_wait(struct sem *s);
/* Perform V operation: wake other processors. */
void sem_inc(struct sem *s);

#endif