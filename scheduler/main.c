#include <stdlib.h>
#include <stdio.h>
#include "sched.h"


child(){
	printf("In the child\n");
	while(1){
	}
	
}

parent(){
	printf("In the parent\n");
	while(1){
	}
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