////////////////////
//Sameer Chauhan  //
//ECE357          //
//CatGrepMore Program //
////////////////////  

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h> 
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#define BUFFER_SIZE 16384

int numFiles = 0;

void int_handler(int sig){
	fprintf(stderr,"\n\nProcess interrupted\n");
	fprintf(stderr,"Files processed: %d\n",numFiles);
	exit(-1);
}
	
int main (int argc, char **argv){

	if(argc<3){
		fprintf(stderr,"Enter the right number of arguments\n");
		exit(-1);
	}

	// Handling interrupts
	signal(SIGINT,int_handler);

	int index,grepPid,morePid,status;



	//File Descriptors
	int inFile;
	int fds[2];
	char* grepCmd[4] = {"grep",argv[1],NULL,NULL};

				

	for(index = 2; index<argc; index++){
		printf("Grepping %d out of %d '%s' with pattern: '%s'\n",index-1,argc-2,argv[index],argv[1]);
		grepCmd[2] = argv[index];
		// if( (inFile = open(argv[index], O_RDONLY, 0666))<0){
		// 	fprintf(stderr,"Error occured while opening %s: %s\n", argv[index], strerror(errno));
		// 	return -1;
		// }
		if (pipe(fds)<0){
			perror("Can't create pipe");
			return -1;
		}	
	
		switch((grepPid = fork())){
			case -1:
				perror("Failed to create fork ");
				exit(-1);
				break; 
			case 0: //Child process
				signal(SIGINT,int_handler);
				close(fds[0]); //Close unused read end;
				if(dup2(fds[1],1)<0){
					fprintf(stderr,"Cannot dup stdout to pipe fds[1]\n",strerror(errno));
					exit(-1);
				}
				execvp(grepCmd[0],grepCmd);
				break;
			default: //Parent process
				waitpid(grepPid,&status,WUNTRACED);
				close(fds[1]);
				numFiles++;
				break;
		}
		
		switch ((morePid = fork())){
			case -1:
				perror("Failed to create fork ");
				exit(-1);
				break; 
			case 0: //Child process
				close(fds[1]); // Close unused write end. 
				if(dup2(fds[0],0)<0) {
					fprintf(stderr,"Cannot dup stdin to pipe fds[0]\n",strerror(errno));
					exit(-1);
				}
				execlp("more","more",NULL);
				break;
			default: //Parent process
				waitpid(morePid,&status,0);
				close(fds[0]);
				break;
		}
	}
 return 0;
}