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


void doCmd(char* line){
	char *cmd;
	char *cmdArg;
	char *cmdList[MAX_LENGTH];
	if ((cmd = strtok(line, DELIMS))) {
      errno = 0;
      cmdList[0] = cmd;
      if (strcmp(cmd, "cd") == 0) {
        cmdArg = strtok(0, DELIMS);

        if (!cmdArg) 
        	fprintf(stderr, "cd missing argument.\n");
        else 
        	chdir(cmdArg);
      } 
      else {
        printf("%s\n",line);
      } 
	}

}






int main(int argc, char *argv[]) {
  char *cmd;
  char *cmdArg;
  char line[MAX_LENGTH];
  int pid, status, count;
  char *cmdList[MAX_LENGTH];
  int ofd,ifd;
  char *fileName;
  struct rusage ru;
  pid_t cpid;
  int  cstart, cend;
  char *split;
  FILE* inFile;


	int bufferSize = 1024;  // Default buffer size
	char* bufferPtr = NULL;

	if(argc> 1){
		if((inFile = fopen(argv[1],"r") ) != NULL){
			while(fgets(line,sizeof(line),inFile) != NULL){
				if(line[0] != '#'){
					doCmd(line);
				}
			}
			fclose(inFile);
		}	
		else{
			perror("Open failed ");
		}
	}
	else{
		while(1){
			printf("mysh> ");
			if(fgets(line, MAX_LENGTH,stdin) != NULL){
				doCmd(line);
			} 
			else
				break;
		}
	}

// line = line of input
// cmd = name of cmd 
// cmdArg = arguments of cmd
// cmdList = array to passed into Exec (reset everytime)

  while (1) {
  	//Clear cmdList at each command
  	memset(&cmdList[0], 0, sizeof(cmdList));

    printf("mysh> ");
    //Read input
    if (!fgets(line, MAX_LENGTH, stdin)) 
    	break;

    // Get parameter stuff

    if ((cmd = strtok(line, DELIMS))) {
      errno = 0;
      cmdList[0] = cmd;


      // First condition is if cd is called. Make it work with exec
      if (strcmp(cmd, "cd1") == 0) {
        cmdArg = strtok(0, DELIMS);

        if (!cmdArg) 
        	fprintf(stderr, "cd missing argument.\n");
        else 
        	chdir(cmdArg);
      } 
      else if (strcmp(cmd, "exit") == 0) {
        break;
      } 
      ///CD Nonsense
      
      else {
      	printf("Executing command %s with arguments", cmd);
      	count = 1;
      	while(cmdArg = strtok(0,DELIMS)){
      		printf(" '%s'",cmdArg);
      		cmdList[count] = cmdArg;
      		count++;
      	}
      	printf("\n");
      	cmdList[count] = NULL;
      	switch(pid = fork()){
      		case -1:
      			perror("Failed to create fork ");
      			exit(1);
      			break; // Not reached
      		case 0: //Child process
      			cstart = clock();
      			if(execvp(cmdList[0],cmdList) <0){
      				perror("Failed to exec ");
      				exit(-1);
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
      }
      
      if (errno) 
      	perror("Command failed ");
    }
  }

  return 0;
}
