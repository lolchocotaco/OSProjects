///////////////////////////
// Sameer Chauhan        //
// ECE357                //
// scheduler (Header)   //
///////////////////////////



#ifndef SCHED_H
#define SCHED_H

#include "savectx.h"


#define SCHED_NPROC 	512
#define SCHED_READY 	0
#define SCHED_RUNNING 	1
#define SCHED_SLEEPING 	2
#define SCHED_ZOMBIE 	-1


/* 
	Structure used for each simulated task 
	Contains:
		State
		Priority
		Accumulated
		CPU Time
		Stack Address
		Others

*/
struct sched_proc {
	int state;
	int pid;
	int ppid;
	int priority;
	int totTime;
	int cpuTime;
	int nice;
	void *stackPtr;
	struct savectx *ctx;
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
void sched_init(void (*init_fun)());

int sched_fork();

int sched_exit(int code);

int sched_wait(int *exit_code);

int sched_nice(int niceval);

int sched_getpid();

int sched_gettick();

void sched_ps();

void sched_switch();

void sched_tick();





#endif