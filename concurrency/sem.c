///////////////////////////
// Sameer Chauhan        //
// ECE357                //
// Semaphores  			//
///////////////////////////


#include "sem.h"


void sigHandler(int signal){
	printf("Waking up...\n");
}


void sem_int(struct sem *s, int count){
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
			s->waiting[myprocnum] = p;
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
	printf("sem_inc\n");
}