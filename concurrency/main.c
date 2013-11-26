#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "sem.h"
#include "fifo.h"

struct fifo *f;
int main(int argc, char**argv){
	if(argc != 3){
		fprintf(stderr,"Please put the correct number of arguments\n");
		exit(-1);
	}
	int nP,numProc,nW, numWords;
	unsigned long readVal;
	unsigned int pidz;
	int seqNum;
	int tots =0 ;

	numProc = atoi(argv[1]);
	numWords= atoi(argv[2]);


	/* Create Shared FIFO */
	f = mmap (NULL, sizeof(struct fifo), PROT_READ | PROT_WRITE,MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	if(f == NULL){
		perror("mmap error ");
		exit(-1);
	}
	
	fifo_init(f);
	printf("Spawning %d process sending %d words\n",numProc,numWords);
	my_procnum = 0;
	
	/* Writing
		Fork Processes and in each process write word to FIFO. 
		The number of processes and number of words is specified by the user.
		The word sent is the process number in the first 6 bits, and the sequence number in the remaining
	*/
	for(nP = 0; nP < numProc; nP++){
		switch(fork()){
			case -1:
				perror("Forking failed");
				break;
			case 0:
				my_procnum = nP;
				for(nW = 0; nW < numWords; nW++){
					fifo_wr(f,(nP+1)|((nW + 1)<<6));
				}
				printf("Process #%d wrote successfully: %d\n",nP, (numWords)*(numWords+1)/2);
				exit(0);
				break;
		}
	} 
	
	/* Reading 
		Reads from the fifo for the same number of processes that were initialized. 
	*/
	for(nP = 0; nP<numProc; nP++ ){
		for(nW = 0; nW<numWords; nW++){
			readVal=fifo_rd(f);
			seqNum = readVal>>6;
			pidz = ~((seqNum << 6) & readVal);
			tots +=seqNum;
		}
	}

	printf("Succesfully sent: %d using %d processes\n", (numWords)*(numWords+1)/2*numProc,nP);
	printf("Recv'd total of : %d from  %d processes\n",tots,nP);
	return 0;
}