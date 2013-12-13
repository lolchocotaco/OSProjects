///////////////////////////
// Sameer Chauhan        //
// ECE357                //
// scheduler (Header)   //
///////////////////////////



#ifndef SCHED_H
#define SCHED_H

#include "savectx.h"
#include "jmpbuf-offsets.h"


#define SCHED_NPROC 	511
#define SCHED_READY 	0
#define SCHED_RUNNING 	1
#define SCHED_SLEEPING 	2
#define SCHED_ZOMBIE   -1
#define TRUE			1
#define FALSE			0
#define CHILD_RUNNING	1
#define	CHILD_DEAD	   -1
#define CHILD_DNE		2
#define STATUS (const char*[4]){"READY","RUNNER","SLEEP","ZOMBIE"}  

/* 
	Structure used for each simulated task 
	Contains:
		State
		Priority
		Accumulated
		CPU Time
		Stack Address
		Context switch

*/
struct sched_proc {
	int state;
	int pid;
	int ppid;
	int nice;     // Static Priority
	int priority; // Dynamic Priority
	int cpuTicks;
	int curTicks;
	int oneExitCode; //Single Child Exit COde
	void *stackPtr;
	struct savectx ctx;
};


/*
	Used for each event/wakeup queue

 */
struct sched_waitq {


};

/*
	Called once to initialize scheduling system. 
	Sets periodic interval timer
		Sets sched_tick as signal handler
	Initial task has PID of 1
		Makes it runnable and transfers execuation to init_fn
 */
void sched_init(void (*init_fn)());

int sched_fork();

int sched_exit(int code);

int sched_wait(int *exit_code);

void sched_nice(int niceval);

int sched_getpid();

int sched_getppid();

int sched_gettick();

void sched_ps();

void sched_switch();

void sched_tick();





#endif