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
#include <sys/ioctl.h>
#include <signal.h>
#include <fcntl.h>         

int writeSize;
int bufferSize = 1024;//16384;  // Default buffer size
char* bufferPtr = NULL;



int numFiles = 0;
int bytesWritten = 0;
void int_handler(int sig){
	fprintf(stderr,"\n\nProcess interrupted\n");
	fprintf(stderr,"Files processed: %d with %d bytes\n",numFiles,bytesWritten);
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

	int bytesAvailable,err;

	//File Descriptors
	int inFile;
	int grepfds[2],morefds[2];
	char* grepCmd[4] = {"grep",argv[1],NULL,NULL};


	bufferPtr = malloc(bufferSize);
	if(bufferPtr == NULL){
		fprintf(stderr,"Cannot allocate buffer of size %d\n",bufferSize);
		return -1;
	}
				

	for(index = 2; index<argc; index++){
		printf("Grepping %d out of %d '%s' with pattern: '%s'\n",index-1,argc-2,argv[index],argv[1]);
		if (pipe(grepfds) < 0 || pipe(morefds) <0){
			perror("Can't create pipe");
			return -1;
		}	
			
		switch ((grepPid = fork())){
			case -1:
				perror("Failed to create fork ");
				exit(-1);
				break; 
			case 0: //Child process
				close(grepfds[1]);
				close(morefds[0]);

				// output grep to other pipe
				if(dup2(morefds[1],1)<0){
					fprintf(stderr,"Cannot dup stdin to pipe fds[0]\n",strerror(errno));
					exit(-1);
				}
				// Read from pipe as stdin
				if(dup2(grepfds[0],0)<0) {
					fprintf(stderr,"Cannot dup stdin to pipe fds[0]\n",strerror(errno));
					exit(-1);
				}
				printf("doing grep");
				execvp(grepCmd[0],grepCmd);
				break;
			default: //Parent process
				//Close unused pipes
				close(grepfds[0]);
				// Open up file for reading
				if( (inFile = open(argv[index], O_RDONLY, S_IREAD | S_IWRITE))<0){
					fprintf(stderr,"Error occured while opening %s: %s\n", argv[index], strerror(errno));
					return -1;
				}
				// Write to pipe
				while((writeSize = read(inFile,bufferPtr,bufferSize)) > 0){
					if( write(grepfds[1],bufferPtr,writeSize) != writeSize){
      					perror("Error occured while writing to pipe (Partial Write)");
      					exit(-1);
    				}
    				bytesWritten = bytesWritten + writeSize;
    			}
    			if(writeSize == -1){
    				perror("Error occured while writing to pipe");
    			}
				close(grepfds[1]);
				waitpid(grepPid,&status,0);
				close(morefds[1]);
				close(inFile);

				numFiles++;
				break;
		}


		switch((morePid = fork())){
			case -1:
				perror("Failed to create fork ");
				exit(-1);
				break; 
			case 0: //Child process
				close(morefds[1]);
				if(dup2(morefds[0],0)<0){
					fprintf(stderr,"Cannot dup to grepfds\n",strerror(errno));
					exit(-1);
				}
				execlp("more","more",NULL);
				break;
			default: //Parent process
				waitpid(morePid,&status,0);
				close(morefds[0]);
				break;
		}

	}
 return 0;
}