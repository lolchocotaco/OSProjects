#ifndef SEM_H
#define SEM_H

#include <sys/types.h>
#include <sys/sem.h>


/* One semaphore structure for each semaphore in the system. */
struct sem {
    pid_t   sempid;         /* pid of last operation */
    unsigned short int semval;         /* current value */
    unsigned short int semncnt;        /* num procs awaiting increase in semval */
    unsigned short int semzcnt;        /* num procs awaiting semval = 0 */
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