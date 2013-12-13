#include <stdlib.h>
#include <stdio.h>
#include "sched.h"


child(){
	printf("In the child\n");
	unsigned int count;
	for (count = 0; count < 1000000000; count++){}
	sched_exit(1);
	
}

parent(){
	printf("In the parent\n");
	int childRet;

	unsigned int count;
	for (count = 0; count < 2000000000; count ++){	
	}
	sched_wait(&childRet);
	fprintf(stderr,"Child returned %d\n",childRet);
}



void testFunc(){
	switch( sched_fork() ){
		case 0:
			child();
			break;
		default:
			parent();
			break;
	}
	exit(0);
}

int main(int argc, char **argv){

	printf("Making Schedule---\n");
	sched_init(testFunc);

	return 0;
}