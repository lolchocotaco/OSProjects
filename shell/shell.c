////////////////////
//Sameer Chauhan  //
//ECE357          //
//Find Program //
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
#include <time.h>	
#include <grp.h>
extern int errno;

#define MAX_LENGTH 1024
#define DELIMS " \t\r\n"

int main(int argc, char *argv[]) {
  char *cmd;
  char *cmdArg;
  char line[MAX_LENGTH];
  int pid, status, done, count;
  char *cmdList[MAX_LENGTH];


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
      if (strcmp(cmd, "cd") == 0) {
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
      	printf("Command %s\n", cmd);
      	count = 1;
      	while(cmdArg = strtok(0,DELIMS)){
      		cmdList[count] = cmdArg;
      		count++;
      	}
      	cmdList[count] = NULL;
      	// char *argvv[] = {cmd, "-l", NULL};
      	switch(pid = fork()){
      		case -1:
      			perror("Failed to create fork: ");
      			exit(-1);
      		case 0:
      			if(execvp(cmdList[0],cmdList) <0){
      				perror("Failed to exec: ");
      				exit(-1);
      			}
      		default:
      			done = wait(&status);
      	}
      }
      

      if (errno) 
      	perror("Command failed: ");
    }
  }

  return 0;
}