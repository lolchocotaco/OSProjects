////////////////////
//Sameer Chauhan  //
//ECE357          //
//Copycat Program //
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

extern int errno;

int writeSize;
int bufferSize = 1024;  // Default buffer size
char* bufferPtr = NULL;


/* Copycat function
  Returns 1 for input file error:
  Returns 2 for output file error:
  Returns 0 for no error;
  Inputs:
    File descriptors for input and output. 
 */
int copycat(int inFile, int outFile){
  
  while((writeSize = read(inFile,bufferPtr,bufferSize)) != 0){
    if(writeSize == -1){
      return 1;
    }
    if( write(outFile,bufferPtr,writeSize) != writeSize){
      return 2;
    }
  }
  if(writeSize ==-1)
    return 1;
  else
    return 0;
}

int main (int argc, char **argv){
 int oflag = 0;
 int bflag = 0;

 // unsigned int writeSize;


 int index;
 int optVal;

 //File Descriptors
 int inFile;
 int outFile;
 char *outName =NULL;

 opterr = 0;
// Find the operators
 while ((optVal = getopt (argc, argv, "b:o:")) != -1)
   switch (optVal)
     {
     case 'o':
       oflag = 1;
       outName = optarg;
       break;
     case 'b':
       bflag = 1;
       bufferSize = atoi(optarg); // Save the buffer size
       break;
     case '?':
       if (optopt == 'b' || optopt =='o')
         fprintf (stderr, "Option -%c requires a additional arguments.\n", optopt);
       else if (isprint (optopt)) //Checks if the character is printable. 
         fprintf (stderr, "Unknown option `-%c'.\n", optopt);
       else
         fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
       return -1;
     default:
       abort ();
     }


/*
  Loads the Output file. If none is specified, stdout is chosen;
 */
if(oflag ==1  && outName !=NULL){
  if (  (outFile = open(outName, O_WRONLY |O_CREAT | O_TRUNC, S_IREAD | S_IWRITE ) )== -1){
    fprintf(stderr,"Error occured while opening %s: %s\n", outName, strerror(errno));
    return -1;
  }
}
else{
  outFile = 1;
}

/* Allocates proper memory */
bufferPtr = malloc(bufferSize);
if(bufferPtr == NULL){
  fprintf(stderr,"Cannot allocate buffer of size %d\n",bufferSize);
  return -1;
}
/*
  If no options are supplied the input is stdin.
  goes through remaining arguments and applyies copycat function. 

 */
if(optind == argc){
  copycat(0,outFile);
}
else{
  for(index = optind; index <argc; index++){
    if( argv[index][0] == '-')
      inFile = 0;
    else if( (inFile = open(argv[index], O_RDONLY, S_IREAD))== -1){
      fprintf(stderr,"Error occured while opening %s: %s\n", argv[index], strerror(errno));
      return -1;
    }
     
    int returnVal = copycat(inFile,outFile);
    switch (returnVal){
      case 0:
        break;
      case 1:
        fprintf(stderr,"Error occured while opening %s: %s\n", argv[index], strerror(errno));
        return -1;
      case 2:
        fprintf(stderr,"Error occured while opening %s: %s\n", outName, strerror(errno));
        return -1;
      default:
        break;
    }

  }
}




// if(optind==argc){
//   printf("Type Input:\n");
//   writeSize = read(0,bufferPtr,bufferSize);
//   if( write(outFile,bufferPtr,writeSize) != writeSize){
//     fprintf(stderr,"Error occured while opening %s: %s\n",outName, strerror(errno));
//     return -1;
//   }
// } 
// else{
//   for (index = optind; index < argc; index++){
//     printf ("Input File: %s\n", argv[index]);
//     if( argv[index][0] == '-'){
//       writeSize = read(0,bufferPtr,bufferSize);
//       if( write(outFile,bufferPtr,writeSize) != writeSize){
//         fprintf(stderr,"Error occured while opening %s: %s\n",outName, strerror(errno));
//         return -1;
//       }      
//     } 
//     else{
//       inFile = open(argv[index], O_RDONLY, S_IREAD);
//       if(inFile != -1){
//         writeSize = read(inFile, bufferPtr, bufferSize);
//         if(outFile !=-1 && write(outFile, bufferPtr, writeSize) == writeSize){
//           close(inFile);
//         }
//         else{
//           fprintf(stderr,"Error occured while opening %s: %s\n",outName, strerror(errno));
//           close(outFile);
//           return -1;
//         }
//        } 
//       else{
//         fprintf(stderr, "Error occured while opening %s: %s\n",argv[index],strerror(errno));
//         close(outFile);
//         return -1;
//        }
//      }

//   }
// }
// printf("-----------------------\n Copycat Complete: %d\n", outFile);

 return 0;
}