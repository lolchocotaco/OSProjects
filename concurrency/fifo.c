///////////////////////////
// Sameer Chauhan        //
// ECE357                //
// Fifo  			//
///////////////////////////


#include "fifo.h"


void fifo_init( struct fifo *f){
	f->rStart = 0;
	f->wStart = 0;
	sem_init(&f->access,1);
	sem_init(&f->read,MYFIFO_BUFSIZ);
	sem_init(&f->write, MYFIFO_BUFSIZ);
}