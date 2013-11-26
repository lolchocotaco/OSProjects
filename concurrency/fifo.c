///////////////////////////
// Sameer Chauhan        //
// ECE357                //
// Fifo  				 //
///////////////////////////


#include "fifo.h"


void fifo_init( struct fifo *f){

	f->rStart = 0;
	f->wStart = 0;
	sem_init(&(f->access),1);
	sem_init(&(f->read), 0);
	sem_init(&(f->write), MYFIFO_BUFSIZ);
}


void fifo_wr(struct fifo *f, unsigned long d){
	// while(1){
		sem_wait(&(f->write));		// write lock
		sem_wait(&(f->access));		// buffer lock
		// if(sem_try(&(f->access))){
			f->buf[f->wStart] = d;
			f->wStart = (f->wStart + 1 ) % MYFIFO_BUFSIZ;
			sem_inc(&(f->read));
			sem_inc(&(f->access));
		// 	return;
		// } else{
		// 	sem_inc(&(f->write));
		// }
	// }
}


unsigned long fifo_rd( struct fifo *f){
	unsigned long returnVal;
	// while(1){
		sem_wait(&(f->read));
		sem_wait(&(f->access));
		// if(sem_try(&(f->access))){
			returnVal = f->buf[f->rStart];
			f->rStart = (f->rStart + 1 ) % MYFIFO_BUFSIZ;
			sem_inc(&(f->write));
			sem_inc(&(f->access));	
			return returnVal;
		// } else{
		// 	sem_inc(&(f->read));
		// }
		
	// }
	
}