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
#include <string.h>

#include "adjstack.c"
#include "sched.h"
#define STACK_SIZE 65536
#define TICKWINDOW 10

int F_NEED_RESCHED = FALSE;
int numTicks = 0;
struct sched_proc *curProc;
struct sched_proc* q[SCHED_NPROC];
sigset_t mask;

void sched_init(void (*init_fn)()){
	// Set interval timer
	struct itimerval timer;
	timer.it_value.tv_sec = 0;
	timer.it_value.tv_usec = 5000; // 100 ms
	timer.it_interval.tv_sec = 0;
	timer.it_interval.tv_usec = 5000; //100 ms
	if (setitimer(ITIMER_VIRTUAL, &timer, NULL) < 0) {
		perror("Cannot set timer: ");
		exit(-1);
	}

	// Make the signal mask 
	sigemptyset(&mask);
	sigaddset(&mask,SIGVTALRM);

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
	// Set values within current Process
	curProc -> pid = 1;
	curProc -> ppid = 0;
	curProc -> state = SCHED_RUNNING;
	curProc -> stackPtr = newSP;

	// Set context switch
	struct savectx ctx;
	ctx.regs[JB_SP] = curProc -> stackPtr + STACK_SIZE;
	ctx.regs[JB_BP] = curProc -> stackPtr + STACK_SIZE;
	ctx.regs[JB_PC] = init_fn;

	// Add process to queue
	q[1] = curProc;
	
	restorectx(&ctx, 1);

	return;
}


int sched_fork(){
	sigprocmask(SIG_BLOCK,&mask,NULL);
	struct sched_proc *newProc;
	
	// Find next PID to allocate exit it cannot
	int pidVal;
	for (pidVal = 1; pidVal < SCHED_NPROC; pidVal++){
		if (q[pidVal] == NULL){
			break;
		}
	}
	if (pidVal >= SCHED_NPROC){
		fprintf(stderr, "Cannot Schedule Process");
		sigprocmask(SIG_UNBLOCK,&mask,NULL);
		return -1;
	}

	// Create new Proccess
	newProc = (struct sched_proc *) malloc(sizeof(struct sched_proc));
	if (newProc == NULL){
		fprintf(stderr,"Could not malloc process\n");
		sigprocmask(SIG_UNBLOCK,&mask,NULL);
		return -1;
	}

	// Allocate new memory region for child process
	void *newSP;
	if ((newSP=mmap(0, STACK_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0 , 0)) == MAP_FAILED){
		perror("Cannot malloc stack: ");
		sigprocmask(SIG_UNBLOCK,&mask,NULL);
		return -1; // Free older allocations
	}

	// Copy parent memory into new memory region
	memcpy(newSP, curProc->stackPtr, STACK_SIZE);

	// Adjust new memory region pointers
	adjstack(newSP, newSP+STACK_SIZE, newSP-curProc->stackPtr);

	// Initialize newProc values (make ready)
	newProc->pid = pidVal;
	newProc->ppid = curProc -> pid;
	newProc->state = SCHED_READY;
	newProc->stackPtr = newSP;
	q[pidVal] = newProc; 

	// Save context for child
	if (savectx(&newProc->ctx)  == 0){
		newProc->ctx.regs[JB_SP] += newSP - curProc -> stackPtr ;
		newProc->ctx.regs[JB_BP] += newSP - curProc -> stackPtr ;
		sigprocmask(SIG_UNBLOCK,&mask,NULL);
		return(pidVal);
	} 
	//In child 
	sigprocmask(SIG_UNBLOCK,&mask,NULL);
	return 0;
}

int sched_exit(int code){
	// curProc -> state = SCHED_ZOMBIE;
	return 0;
}

int sched_wait(int *exit_code){
	return 0;
}

/* Done */
void sched_nice(int niceval){
	if (niceval >= 19){
		curProc -> nice = 19;
	} else if ( niceval <= -20 ){
		curProc -> nice = -20;
	} else {
		curProc -> nice = niceval;
	}
	return;
}

/* Done */
int sched_getpid(){
	return curProc -> pid;
}

/* Done */
int sched_getppid(){
	return curProc -> ppid;
}

/* Done */
int sched_gettick(){
	return numTicks;
}

void sched_ps(){
	printf("pid \t ppid \t state \t Stack BP \t Wait Queue \t S_P \t D_P \t CPU \n");
	printf("--- \t ---- \t ----- \t -------- \t ---------- \t --- \t --- \t --- \n");
	int n;
	for (n = 0; n < SCHED_NPROC; n++){
		if (q[n] != NULL){
			printf("%d \t %d \t %d \t %p \t %p \t %d \t %d \t %d \n",
				q[n]-> pid, 
				q[n]-> ppid, 
				q[n]-> state, 
				q[n]-> stackPtr,
				q[n], 
				q[n]-> nice, 
				q[n]-> priority, 
				q[n]-> cpuTicks);
		}
	}
	return;
}

void sched_switch(){ 
	int pid;

	// Scheduling algorithm (Choose Next Best State)
	for (pid = 0; pid < SCHED_NPROC; pid++){
		if (q[pid] != NULL && q[pid]->state == SCHED_READY){
			break;
		}
	}
	if (pid == SCHED_NPROC){ //No one is ready
		sigprocmask(SIG_UNBLOCK,&mask,NULL);
		return;
	}

	//
	if (savectx(&curProc->ctx) == 0){
		curProc -> state = SCHED_READY;
		curProc = q[pid];
		curProc -> state = SCHED_RUNNING;

		sched_ps();
		sigprocmask(SIG_UNBLOCK,&mask,NULL);
		restorectx(&curProc->ctx,1);
	} else{
		sigprocmask(SIG_UNBLOCK,&mask,NULL);
	}

	return;

}

void sched_tick(){

	numTicks++;
	curProc -> cpuTicks++;
	printf("numTicks: %d\n",curProc->cpuTicks);
	sched_switch();

	return;
}
