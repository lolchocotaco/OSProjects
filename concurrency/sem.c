///////////////////////////
// Sameer Chauhan        //
// ECE357                //
// Semaphores  			//
///////////////////////////


#include "sem.h"


void sigHandler(int signal){
	printf("Waking up...\n");
}


void sem_init(struct sem *s, int count){
	s->lock = 0;
	s->count = count; 
}


int sem_try(struct sem *s){
	//Wut
}
void sem_wait(struct sem *s){
	while(1){
		while(tas( &(s->lock)) !=0) {} // Get access to semaphore

		if(s->count >0){ //Has resources
			(s->count)--;
			s->lock = 0;
			return;
		}
		else{	//No resources
			pid_t p = getpid();
			s->waiting[my_procnum] = 1;
			s->sempids[my_procnum] = p; // Save real PID
			printf("sleepingPID: %d\n", p);

			// Go to sleep
			sigset_t mask;
			sigfillset(&mask);
			sigdelset(&mask,SIGUSR1);
			sigdelset(&mask,SIGINT);
			sigprocmask(SIG_BLOCK, &mask, NULL);
			signal(SIGUSR1,sigHandler);
			s->lock = 0;
			sigsuspend(&mask);
			sigprocmask(SIG_UNBLOCK, &mask, NULL);
		}
	}
}
void sem_inc(struct sem *s){
	while(tas( &(s->lock)) !=0) {}

	int ind;
	s->count++;
	for(ind = 0; ind< N_PROC; ind++){
		if(s->waiting[ind]){
			s->waiting[ind] = 0;
			kill(s->sempids[ind],SIGUSR1);
		}
	}

	s->lock = 0;
}