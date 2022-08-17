#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>



int main(int argc, char* argv[]){
//Robert Miller
//I pledge my honor that I have abided by the Stevens Honor System
    if(argc < 2){
        printf("Error: No Directory Given.\n");
        return EXIT_FAILURE;
    }
    if(argc > 2){
        printf("Error: Too many args.\n");
        return EXIT_FAILURE;
    }
    int fd[2]; //pipe for child output to granchild input
    int cd[2]; //pipe for grandchild output to parent
    if(pipe(fd) < 0){
        printf("Error: Pipe failed.\n");
        return EXIT_FAILURE;
    }
    if(pipe(cd) < 0){
        printf("Error: Pipe failed.\n");
        return EXIT_FAILURE;
    }
    //fork 1 for ls child
    pid_t pid = fork();
    if(pid == -1){
        printf("Error: fork() failed.\n");
        return EXIT_FAILURE;
    }
    if(pid == 0){ //child, make sure to exit
    close(fd[0]); //close read end
    close(cd[1]); //close other pipe
    close(cd[0]);
    dup2(fd[1], 1); //make the pipe's write into execv's output
    if(execl("/bin/ls", "ls", "-l", argv[1], (char *) 0) == -1){
        fprintf(stderr, "Error: ls -l failed. %s.\n", strerror(errno));
        exit(1);
    }
    exit(0);
    }

    if(pid > 0){ //parent, wait for child
    int status;
    waitpid(pid, &status, 0); //end function if ls fails
    if(WEXITSTATUS(status) == 1){
        return EXIT_FAILURE;
    }
    close(fd[1]);
    pid_t pid2 = fork();
    if(pid2 == -1){
        printf("fork() error.\n");
        return EXIT_FAILURE;
    }
    if(pid2 == 0){ //second child
        close(cd[0]); //doesnt read this pipe
        dup2(fd[0], STDIN_FILENO); //set in to 
        dup2(cd[1], STDOUT_FILENO); 
        if(execl("/bin/grep", "grep", "^d", (char *) 0) == -1){
            fprintf(stderr, "Error: grep() failed. %s.\n", strerror(errno));
            exit(1);
        }
        exit(0);
    }
    if(pid2 > 0){
        
        int status2;
        waitpid(pid2, &status2, 0);
        if(WEXITSTATUS(status2) == 1){
            return EXIT_FAILURE;
        }
        close(fd[0]);
        close(cd[1]);
        dup2(cd[0], STDIN_FILENO);
        int temp = 0;
        int count = 0; //since there will always be at least 1 \n
        char buf[5000];
        char args[5000];
        while(temp = read(cd[0], buf, sizeof(buf)) > 0){
            printf("%s", buf);
            strcat(args, buf);
        }
        char * token = strtok(args, "\n");
        while(token != NULL){
            count++;
            token = strtok(NULL, "\n");
        }
        printf("Total amount of subdirectories is: %d\n", count);
    }
    }

    return EXIT_SUCCESS;
}