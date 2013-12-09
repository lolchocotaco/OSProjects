////////////////////
//Sameer Chauhan  //
//ECE357          //
//Shell Program //
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
#include <dirent.h>
#include <limits.h> 		/*defines PATH_MAX */
#include <pwd.h>
#include <sys/wait.h>	
#include <grp.h>
#include <time.h>
#include <sys/resource.h>



#define MAX_LENGTH 1024
#define DELIMS " \t\r\n"

// Redirect fName to stdFD using options
void redirect(char *fName, int stdFD, int options	){
	int fd;
	char* fileName  = strtok(fName,"<>");
	// printf("Redirect to: %s\n",fileName);
	if( (fd = open(fileName,options,S_IREAD | S_IWRITE ) ) <0 ){
		fprintf(stderr, "Cannot open file %s: %s\n",fileName, strerror(errno));
		exit(1);
	}
	if(dup2(fd,stdFD)<0){
		fprintf(stderr,"Cannot dup %s to %d: %s",fileName,stdFD,strerror(errno));
		exit(1);
	}
	close(fd);
}



int main(int argc, char *argv[]) {
	char *cmd;
	char *cmdArg;
	char line[MAX_LENGTH];
	int pid, status, count;
	char *cmdList[MAX_LENGTH];
	
	struct rusage ru;
	int  cstart, cend;
	FILE* input = NULL;

	char *fName;
	int stdFD;
	int options;
	int isRedirect = 0;


	//If there is an argument, open up that file for reading
	if(argc> 1){
		if((input = fopen(argv[1],"r") ) == NULL){
			perror("Open failed ");
			exit(1);
		}
	} else{
		input = stdin;
	}

	// line = line of input
	// cmd = name of cmd 
	// cmdArg = arguments of cmd
	// cmdList = array to passed into Exec (reset everytime)
	printf("mysh $> ");
	while (1) {
		//Reset at each loop though
		errno = 0;
		isRedirect = 0;
		//Read input (if there is nothing, break to end program)
		if (!fgets(line, MAX_LENGTH, input)) {
			printf("\n");
			break;
		}
		//If there is a comment move on. 
		if(line[0] == '#'){
			continue;
		}

		//Clear cmdList at each command
		memset(&cmdList[0], 0, sizeof(cmdList));

		// Get parameter stuff
		char *p = line;
		if ((cmd = strtok_r(p, DELIMS,&p))) {
		  errno = 0;
		  cmdList[0] = cmd;

			fprintf(stderr,"Executing command %s with arguments", cmd);
			count = 1;
			while(cmdArg = strtok_r(0,DELIMS,&p)){
				// Check if argument is for redirection
				if(cmdArg[0] == '<'){
					isRedirect = 1;
					fName = cmdArg;
					stdFD = 0;
					options = O_RDONLY;
					// redirect(cmdArg,0, O_RDONLY);
					// printf("\n\n<Dups: %s\n",strtok(cmdArg,"<>"));
				}
				else if(cmdArg[0] == '>'){
					isRedirect = 1;
					fName = cmdArg;
					stdFD =  1;
					if(cmdArg[1]=='>'){
						options = O_CREAT|O_APPEND|O_WRONLY;
					} else{
						options = O_CREAT|O_TRUNC|O_WRONLY;
					}
					// printf("\n\n>Dups: %s\n",strtok(cmdArg,"<>"));
				}
				else if(cmdArg[0] == '2'){
					isRedirect = 1;
					//Some character stripping  (useless apce)
					char* two = strtok(cmdArg,"<>"); 
					fName = strtok(0,"<>");
					stdFD = 2;
					if(cmdArg[2] == '>'){
						options = O_CREAT|O_APPEND|O_WRONLY;
						// printf("dubs");
					} else{
						options = O_CREAT|O_TRUNC|O_WRONLY;
					}
				}
				else{
			  		fprintf(stderr," '%s'",cmdArg);
			  		cmdList[count] = cmdArg;
			  		count++;
				}
				
			}
			fprintf(stderr,"\n");
			cmdList[count] = NULL;

			switch(pid = fork()){
				case -1:
					perror("Failed to create fork ");
					exit(1);
					break; // Not reached
				case 0: //Child process
					cstart = clock();
					if(isRedirect){
						redirect(fName,stdFD,options);
					}

					if(execvp(cmdList[0],cmdList) <0){
						perror("Failed to exec ");
						exit(1);
					}
					cend = clock();
					break;
				default: //Parent process
					if(wait3(&status,0,&ru)== -1)
						perror("Wait failed ");
					else{
						fprintf(stderr,"Command returned with return code %d,\n",status);
						fprintf(stderr,"consuming %6f real seconds %ld.%6d user, %ld.%6d system \n", 
							(double)(cend-cstart),
							ru.ru_utime.tv_sec, 
							(int)ru.ru_utime.tv_usec,
							ru.ru_stime.tv_sec,
							(int)ru.ru_stime.tv_usec);
					}
					break;
				}
		  if (errno) 
		  	fprintf(stderr,"Command failed %s: %s\n",cmdArg,strerror(errno));
		}
		printf("mysh $> ");
	}

  return 0;
}
