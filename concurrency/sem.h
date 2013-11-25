#ifndef SEM_H
#define SEM_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <signal.h>
#define N_PROC 64

int my_procnum; // Virtual PID

/* Semaphore Structure */
struct sem {
	unsigned int count;
	int waiting[N_PROC];
	pid_t sempids[N_PROC];
	volatile char lock;
};


/* Assembly Code*/
int tas(volatile char *lock);
/* Initialize the sempahore *s with the initial count */
void sem_init( struct sem *s, int count);
/* Attept to atomically decrement the semaphore */
int sem_try(struct sem *s);
/* Perfom operation while blocking until successful*/
void sem_wait(struct sem *s);
/* Perform V operation: wake other processors. */
void sem_inc(struct sem *s);

#endif