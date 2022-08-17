#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>


int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <num forks>\n", argv[0]);
        return EXIT_FAILURE;
    }
    int num_forks = atoi(argv[1]);
     pid_t pid;
    if(num_forks <= 0) {
        fprintf(stderr, "Error: Invalid number of forks '%s'.\n", argv[1]);
        return EXIT_FAILURE;
    }
// Robert, Aman, Joseph
    /*
      TODO:
      Use fork() and execl().

      Create num_forks children processes.
      If any call to fork fails, an error message should be printed and
      the loop should continue.
      Keep track of how many children were actually spawned.
      The error message will be of the form:
      Error: fork[fork_index] failed. %s.
         fork_index will be an integer [1, ..., num_forks]
         %s will be strerror(errno)

      The children should execute randomsleep with no arguments.
      If execl fails, an error message should be printed and
      EXIT_FAILURE should returned.
      The error message will be of the form:
      Error: execl() failed for pid %ld. %s.
         %s will be strerror(errno)
    */
    

    /*
      TODO:
      Use wait().

      We are in the parent here. Wait for all children that were actually
      spawned. As each child terminates, print the message:
      Child with pid <pid> has terminated.
      If an error occurs during waiting, instead print the message:
      Error: wait() failed. %s.
         %s will be strerror(errno)
    */

    for(int i = 0; i < num_forks; i++){
        pid_t pid = fork();
        if(pid < 0){
            //error
            fprintf(stderr, "Error: fork[%d] failed. %s.\n", i, strerror(errno));
            return EXIT_FAILURE;
        }
        if(pid == 0){
            //child
            if(execl("/home/robert/CS 392/randomsleep", "randomsleep", (char*) 0) < 0){
                fprintf(stderr, "Error: execl() failed for pid %d. %s.\n", pid, strerror(errno));
                return EXIT_FAILURE;
            }
        }
        if(pid > 0){
            //parent
            if(waitpid(pid, NULL, 0) < 0){
                fprintf(stderr, "Error: wait() failed. %s.\n", strerror(errno));
            }
            printf("Child with pid %d has terminated.\n", i);
        }
    }
    printf("All children are done sleeping.\n");
    return EXIT_SUCCESS;
}
