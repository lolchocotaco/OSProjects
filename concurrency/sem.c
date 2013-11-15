///////////////////////////
// Sameer Chauhan        //
// ECE357                //
// Semaphores  			//
///////////////////////////

#include <stdio.h>
#include "sem.h"



void sem_int(struct sem *s, int count){
	s->semval = count; 
}
int sem_try(struct sem *s){
	if(tas(&s->semval) >0)
		return 1;
	else
		return 0;
}
void sem_wait(struct sem *s){
	printf("sem_wait\n");
}
void sem_inc(struct sem *s){
	printf("sem_inc\n");
}