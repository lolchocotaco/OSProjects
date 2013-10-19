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
#include <fcntl.h>         /* defines options flags */
#include <sys/types.h>     /* defines types used by sys/stat.h */
#include <sys/stat.h>      /* defines S_IREAD & S_IWRITE  */
#include <signal.h>

extern int errno;


void int_handler(int sig){
	fprintf(stderr,"Signal received: %d\n",sig);
	exit(2);
}


int main (int argc, char **argv){

	if(argc<3){
		fprintf(stderr,"Enter the right number of arguments\n");
		exit(-1);
	}

	int index;

	int grepPid,morePid;


	//File Descriptors
	int inFile;
	int fds[2];
	char* grepCmd[4] = {"grep",argv[1],NULL,NULL};
	printf("inputfiles: \n");

	if (pipe(fds)<0){
		perror("Can't create pipe");
		return -1;
	}

	for(index = 2; index<argc; index++){
		printf("Grepping %s with pattern: %s\n",argv[index],argv[1]);
		grepCmd[2] = argv[index];
		// if( (inFile = open(argv[index], O_RDONLY, 0666))<0){
		// 	fprintf(stderr,"Error occured while opening %s: %s\n", argv[index], strerror(errno));
		// 	return -1;
		// }
		switch((grepPid = fork())){
			case -1:
				perror("Failed to create fork ");
				exit(-1);
				break; 
			case 0: //Child process
				printf("in the child\n");
				close(fds[0]); //Close unused read end;
				if(dup2(fds[1],1)<0){
					fprintf(stderr,"Cannot dup stdout to pipe fds[1]",strerror(errno));
					exit(-1);
				}
				close(fds[1]);
				execvp(grepCmd[0],grepCmd);
				
				break;
			default: //Parent process
				printf("In the parent of of grep\n");
				break;

		}
		
		switch ((morePid = fork())){
			case -1:
				perror("Failed to create fork ");
				exit(-1);
				break; 
			case 0: //Child process
				signal(SIGINT,int_handler);
				printf("in the child\n");
				close(fds[1]); // Close unused write end. 
				if(dup2(fds[0],0)<0) {
					fprintf(stderr,"Cannot dup stdin to pipe fds[0]",strerror(errno));
					exit(-1);
				}
				close(fds[0]); 
				execlp("pg","pg",NULL);

				//tell more to read fd[0]
				break;
			default: //Parent process
				printf("In the parent of more\n");
				break;
		}
	}


/*
	Loads the Output file. If none is specified, stdout is chosen;
//  */
// if(oflag ==1  && outName !=NULL){
//   if (  (outFile = open(outName, O_WRONLY |O_CREAT | O_TRUNC, 0666 ) )== -1){
//     fprintf(stderr,"Error occured while opening %s: %s\n", outName, strerror(errno));
//     return -1;
//   }
// }
// else{
//   outFile = 1;
// }

/* Allocates proper memory */
// bufferPtr = malloc(bufferSize);
// if(bufferPtr == NULL){
//   fprintf(stderr,"Cannot allocate buffer of size %d\n",bufferSize);
//   return -1;
// }
/*
	If no options are supplied the input is stdin.
	goes through remaining arguments and applyies copycat function. 

 */
// else{
//   for(index = optind; index <argc; index++){
//     if( argv[index][0] == '-')  //getOpt handles any other (-foo) arguments
//       inFile = 0;
//     else if( (inFile = open(argv[index], O_RDONLY, 0666))== -1){
//       fprintf(stderr,"Error occured while opening %s: %s\n", argv[index], strerror(errno));
//       return -1;
//     }
		 
//     switch (copycat(inFile,outFile)){
//       case 0:
//         break;
//       case 1:
//         fprintf(stderr,"Error occured while opening %s: %s\n", argv[index], strerror(errno));
//         return -1;
//       case 2:
//         fprintf(stderr,"Error occured while opening %s: %s\n", outName, strerror(errno));
//         return -1;
//       default:
//         break;
//     }

//   }
// }

 return 0;
}