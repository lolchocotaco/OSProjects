#include <stdlib.h>
#include <stdio.h>
#include "sched.h"

void testFunc(){
	printf("Hello\n");
	exit(0);
}

int main(int argc, char **argv){

	printf("Making Schedule?\n");
	sched_init(testFunc);
	printf("Pid is %d\n",sched_getpid());

	return 0;
}