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
void pipe_handler(int sig){
	fprintf(stderr,"Broken Pipe\n");
	exit(-1);
}
	
int main (int argc, char **argv){

	if(argc<3){
		fprintf(stderr,"Enter the right number of arguments\n");
		exit(-1);
	}

	// Handling interrupts
	signal(SIGINT,int_handler);
	signal(SIGPIPE,pipe_handler);

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
				if (close(grepfds[1]) <0||  close(morefds[0]) <0 ){
					perror("Cannot close pipes ");
					exit(-1);
				}
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

				if (close(grepfds[0]) <0 ||  close(morefds[1]) <0){
					perror("Cannot close pipes ");
					exit(-1);
				}
				execvp(grepCmd[0],grepCmd);
				break;
			default: //Parent process
				switch((morePid = fork())){
					case -1:
						perror("Failed to create fork ");
						exit(-1);
						break; 
					case 0: //Child process
						if (close(grepfds[0]) <0 || close(morefds[1])<0 || close(grepfds[1])<0){
							perror("Cannot close pipes ");
							exit(-1);
						}
						if(dup2(morefds[0],0)<0){
							fprintf(stderr,"Cannot dup to grepfds\n",strerror(errno));
							exit(-1);
						}
						if( close(morefds[0])<0){
							perror("Cannot close pipes ");
							exit(-1);	
						}
						execlp("more","more",NULL);
						break;
					default: //Parent process
						// close unused pipes
						if(close(morefds[0]) <0 || close(grepfds[0])<0 || close(morefds[1])< 0){
							perror("Error closing pipe: ");
							exit(-1);
						}

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
		    				exit(-1);
		    			}
						if( close(grepfds[1])<0){
							perror("Error closing pipe: ");
							exit(-1);
						}

						numFiles++;
						waitpid(morePid,&status,0);
						break;
				}

				waitpid(grepPid,&status,0);				
				break;
		}
	}
 return 0;
}