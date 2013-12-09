///////////////////////////
// Sameer Chauhan        //
// ECE357                //
// scheduler 			  //
///////////////////////////




#include "sched.h"
#include <stdio.h>
#include <unistd.h>
#include <error.h>


/*
	Called once to initialize scheduling system. 
	Sets periodic interval timer
		Sets sched_tick as signal handler
	Initial task has PID of 1
		Makes it runnable and transfers execuation to init_fn
 */
sched_init(void (*init_fun)()){

}


sched_fork(){

}

sched_exit(int code){

}

sched_wait(int *exit_code){

}

sched_nice(int niceval){

}

sched_getpid(){

}

sched_gettick(){

}

sched_ps(){

}

sched_switch(){

}

sched_tick(){

}


int main(int argc, char **argv){

	return 0;
}