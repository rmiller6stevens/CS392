#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>


int main(int argc, char *argv[]) {

    /*
      TODO:
      Use fork() and pipe(), but NOT popen().

      Create a pipe through which a child process will send two integers to 
      the parent. The parent will add the integers and print the result to 
      the terminal. 

      OPTIONAL:
      Make a second child, which will do the addition. In this case, the parent
      waits for the children to terminate. (See Lab 6 or 7.)
    */
    int fd[2];
    if(pipe(fd) <0 ){
      printf("pipe() error.\n");
      return 1;
    } 
    pid_t pid = fork(); //first child
    if(pid == -1){
      printf("fork() error.\n");
      return 1;
    } else if(pid == 0){ //child
    int a = 1;
    int x = 2;
    write(fd[1], &x, sizeof(int));
    write(fd[1], &a, sizeof(int));
    /*
    Second child
    */
    pid_t pid2 = fork();
    printf("waiting for grandchild\n");
      if(pid2 == -1){
        //fork error
        return 1;
      } else if(pid2 == 0){ //second child
        int s,r;
        read(fd[0], &r, sizeof(int));
        read(fd[0], &s, sizeof(int));
        s=s+r;
        write(fd[1], &s, sizeof(int));
        printf("%d\n", s);
        return 0;
      } else if(pid2 > 0){ //parent of second child
        waitpid(pid2, NULL, 0);
        printf("grandchild done\n");
        return EXIT_SUCCESS;

    } else if(pid > 0){ //parent
      printf("at parent\n");
      waitpid(pid, NULL, 0);
      int m;
      read(fd[0], &m, sizeof(int));
      printf("Int is %d\n", m);
    }
    }
    
    return EXIT_SUCCESS;
}
