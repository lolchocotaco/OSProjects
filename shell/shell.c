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
  char line[MAX_LENGTH];
  int pid;
  int status;

  while (1) {
    printf("$ ");
    if (!fgets(line, MAX_LENGTH, stdin)) break;

    // Parse and execute command
    if ((cmd = strtok(line, DELIMS))) {
      // Clear errors
      errno = 0;

      if (strcmp(cmd, "cd") == 0) {
        char *arg = strtok(0, DELIMS);

        if (!arg) fprintf(stderr, "cd missing argument.\n");
        else chdir(arg);

      } else if (strcmp(cmd, "exit") == 0) {
        break;
      } else {
      	printf("You want to run command %s\n", line);
      	switch(pid = fork()){
      		case -1:
      			perror("Failed to create fork: ");
      			exit(1);
      		case 0:
      			if(execvp(argv[0],&argv[0]) <0){
      				perror("Failed to exec: ");
      				exit(1);
      			}
      		default:
      			wait(&status);
      			printf("I'm the parent\n");
      	}
      }
      

      if (errno) perror("Command failed");
    }
  }

  return 0;
}