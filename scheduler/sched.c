///////////////////////////
// Sameer Chauhan        //
// ECE357                //
// scheduler 			  //
///////////////////////////


#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/mman.h>

#include "sched.h"
#define STACK_SIZE 65536


struct sched_proc *curProc;
struct sched_proc* q[SCHED_NPROC];
sigset_t mask;

void sched_init(void (*init_fun)()){
	// Set interval timer
	struct itimerval timer;
	timer.it_value.tv_sec = 0;
	timer.it_value.tv_usec = 1000; // 100 ms
	timer.it_interval.tv_sec = 0;
	timer.it_interval.tv_usec = 1000; //100 ms

	if (setitimer(ITIMER_VIRTUAL,&timer,NULL) < 0) {
		perror("Cannot set timer: ");
		exit(-1);
	}

	// Set sched_tick() as signal handler
	signal(SIGVTALRM, sched_tick);

	// Create stack
	void *newSP;
	if ((newSP=mmap(0, STACK_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0 , 0)) == MAP_FAILED){
		perror("Cannot malloc stack: ");
		exit(-1); // Free older allocations
	}

	// Malloc first process
	if (!(curProc = (struct sched_proc *) malloc(sizeof(struct sched_proc)))) {
		fprintf(stderr,"Could not malloc for process\n");
		exit(-1);
	}
	curProc -> pid = 1;
	curProc -> state = SCHED_READY;
	curProc -> stackPtr = newSP;
	
	// Probably not needed. 
	// sigemptyset(&mask);
	// sigaddset(&mask,SIGVTALRM);
	// sigprocmask(SIG_BLOCK,&mask,NULL);
	// sigprocmask(SIG_UNBLOCK,&mask,NULL);


	// Add process to queue

	// Set context switch
	struct savectx ctx;
	ctx.regs[JB_SP] = curProc->stackPtr+STACK_SIZE;
	ctx.regs[JB_BP] = curProc->stackPtr+STACK_SIZE;
	curProc -> ctx = &ctx;

	q[1] = curProc;
	return;
}


int sched_fork(){
	return 0;
}

int sched_exit(int code){
	return 0;
}

int sched_wait(int *exit_code){
	return 0;
}

int sched_nice(int niceval){
	return 0;
}

int sched_getpid(){
	return curProc -> pid;
}

int sched_gettick(){
	return 0;
}

void sched_ps(){
	return;
}

void sched_switch(){
	return;
}

void sched_tick(){

}
