#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "shell.h"
#include "boot.h"

#define BUF      128
#define MAX_ARGS    128

/*
*   Main Program
*/
int main(int argc, char *argv[]) {
    //Define local variables
    char *input;
    char **_argv;
    int _argc;

    //size_t object 
    size_t buffer;
    buffer = BUF;

    //Allocate vars to memory.
    _argv = (char **)malloc(sizeof(char *) * MAX_ARGS);

    while (1) {
        
char *ptr;

printf("$ ");
			
input = (char *)malloc(sizeof(char) * BUF);
        
getline(&input, &buffer, stdin); //get input

//Remove '\n' from end of input

input[strcspn(input, "\n")] = '\0';
        //Split  argv and argc into 2 different 
int x;
for (x = 0, ptr = strtok(input, " "); ptr != NULL; ++x, ptr = strtok(NULL, " ")) {
_argv[x] = (char *)malloc(strlen(ptr) + 1);
strcpy(_argv[x], ptr);
        }
_argc = x;

//Check for '>' (output redirection)
int fileoutput, con;
fflush(stdout); //Safety precaution
for (int a = 0; a < _argc; a++) {
if (strcmp(_argv[a], ">") == 0) {

fileoutput = open(_argv[a+1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
con = dup(STDOUT_FILENO); 

if (fileoutput) {
dup2(fileoutput, STDOUT_FILENO);
close(fileoutput);
}
break;
}
}

        //Checks argv and responds
        if (strcmp(_argv[0], "exit") == 0)
            break;
        else if (strcmp(_argv[0], "help") == 0)
            printHelp();
        else if (strcmp(_argv[0], "fmount") == 0) {
            if (_argc < 2) {
                printIncorrect();
                continue;
            }

            if (isMounted()) {
                printf("    `%s` is already mounted.\n", getFile());
                continue;
            }

            mount(_argv[1]);
        } else if (strcmp(_argv[0], "fumount") == 0) {
            if (!isMounted()) {
                printf("    No floppy was detected.\n");
                continue;
            }

            unmount();
        } else if (strcmp(_argv[0], "structure") == 0) {
            if (!isMounted()) {
                printf("    No floppy was detected.\n");
                continue;
            }

            if (strcmp(_argv[1], "-l") == 0)
                structure(1);
            else
                structure(0);
       
 } else if (strcmp(_argv[0], "showsector") == 0) {
            if (!isMounted()) {
                printf("    No floppy was detected.\n");
                continue;
            }

if (_argc < 2) {
printIncorrect();
continue;
            }

showsector(atoi(_argv[1]));
        
} else if (strcmp(_argv[0], "traverse") == 0) {
if (!isMounted()) {
printf("    No floppy was detected.\n");
continue;
}

if (strcmp(_argv[1], "-l") == 0)
traverse(1);
else
traverse(0);
       
 } else if (strcmp(_argv[0], "showfat") == 0) {

if (!isMounted()) {
printf("    No floppy was detected.\n");
continue;
}

showfat();

} else if (strcmp(_argv[0], "showfile") == 0) {
if (!isMounted()) {
printf("    No floppy was detected.\n");
continue;
}

if (_argc < 2) {
printIncorrect();
continue;
}

showfile(_argv[1]);
} else {
printInvalid();
}

fflush(stdout); 
if (con) {
dup2(con, STDOUT_FILENO);
close(con);
}
free(input);		
for (int i = 0; i < _argc; i++)
free(_argv[i]);
}
exit(EXIT_SUCCESS);
}
