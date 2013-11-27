//////////////////////////
// Sameer Chauhan       //
// Problem 5            //
// 1 Billion Iterations //
// OS                   //
//////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>

#define BILLION 1000000000L

void emptyFunction(){}
void simpleFunction(){
	getuid();
}

int main(int argc, char **argv){

	if (argc !=2) {
		fprintf(stderr,"Please put the correct number of inputs (1)\n");
		exit(-1);
	}

	int i, res1, res2;
	struct timespec tStart, tEnd;
	int type = atoi(argv[1]);


	switch(type){
		case 1:
			res1 = clock_gettime(CLOCK_MONOTONIC, &tStart);
			for(i = 0; i < BILLION; i++){}
			res2 = clock_gettime(CLOCK_MONOTONIC, &tEnd);
			break;
		case 2:
			res1 = clock_gettime(CLOCK_MONOTONIC,  &tStart);
			for(i = 0; i <BILLION; i++){
				emptyFunction();
			}
			res2 = clock_gettime(CLOCK_MONOTONIC, &tEnd);
			break;
		case 3:
			res1 = clock_gettime(CLOCK_MONOTONIC,  &tStart);
			for(i = 0; i <BILLION; i++){
				simpleFunction();
			}
			res2 = clock_gettime(CLOCK_MONOTONIC, &tEnd);
			break;
		default:
			fprintf(stderr,"Incorrect argument value\n");
			exit(-1);
	}

	double elapsed = (double)( tEnd.tv_sec - tStart.tv_sec )
		+(double)( tEnd.tv_nsec - tStart.tv_nsec )/(double)BILLION;

	printf("TimeTaken: %lf\n",elapsed);
}
