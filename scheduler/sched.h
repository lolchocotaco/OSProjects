///////////////////////////
// Sameer Chauhan        //
// ECE357                //
// scheduler (Header)   //
///////////////////////////



#ifndef SCHED_H
#define SCHED_H

#define SCHED_NPROC 	512
#define SCHED_READY 	0
#define SCHED_RUNNING 	1
#define SCHED_SLEEPING 	2
#define SCHED_ZOMBIE 	3


/* 
	Structure used for each simulated task 
	Contains:
		State
		Priority
		Accumulated
		CPU Time
		Stack Address
		Others

*/
struct sched_proc {

};


/*
	Used for each event/wakeup queue
 */
struct sched_waitq {


};



#endif