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
	unsigned int pID;
	int seqNum;
	int tots =0 ;

	numProc = atoi(argv[1]);
	numWords= atoi(argv[2]);

	f = mmap (NULL, sizeof(struct fifo), PROT_READ | PROT_WRITE,MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	
	if(f == NULL){
		perror("mmap error ");
		exit(-1);
	}
	
	fifo_init(f);
	printf("Spawning %d process sending %d words\n",numProc,numWords);
	// Reading
	for(nP = 0; nP < numProc; nP++){
		switch(fork()){
			case -1:
				perror("Forking failed");
				break;
			case 0:
				my_procnum = nP;
				for(nW = 0; nW < numWords; nW++){
					// printf("Process Num: %d\t",nP+1);
					// printf("SeqNum: %d\t",nW+1);
					// printf("Sending: %d\n",(nP+1)|((nW + 1)<<6));
					fifo_wr(f,(nP+1)|((nW + 1)<<6));
				}
				exit(0);
				break;
		}
	}
	//Read
	
	for(nP = 0; nP<numProc; nP++ ){
		for(nW = 0; nW<numWords; nW++){
			readVal=fifo_rd(f);
			// printf("Recv'd %ld\n",readVal);
			seqNum = readVal>>6;
			pID = ~((seqNum <<6) & readVal);
			tots +=seqNum;
			// Verifications
		}
	}
	printf("Sum sent: %d | Sum recv: %d\n",(numWords)*(numWords+1)/2*numProc,tots);



	return 0;
}