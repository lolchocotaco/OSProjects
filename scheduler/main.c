#include <stdlib.h>
#include <stdio.h>
#include "sched.h"


child(unsigned long long int val, int niceVal){
	sched_nice(niceVal);
	printf("In the child\n");
	
	unsigned long long int count;
	for (count = 0; count < val; count++){}
	
	sched_exit(1);	
}

parent(){
	sched_nice(3);
	printf("In the parent\n");
	int childRet;

	unsigned long long int count;
	for (count = 0; count < 10000000000; count ++){}
	
	sched_wait(&childRet); 
	fprintf(stderr,"Child returned %d\n",childRet);
}


void testFunc(){
	int secondRet;
	switch( sched_fork() ){
		case 0:
			child(5000000000,-5);
			break;
		default:
			switch(sched_fork()){
				case 0:
					child(20000000000,10);
					break;
				default:
					parent();
					break;
			}
			sched_wait(&secondRet);
			fprintf(stderr,"Child returned %d\n",secondRet);
	}
	exit(0);
}

int main(int argc, char **argv){

	printf("Making Schedule---\n");
	sched_init(testFunc);

	return 0;
}