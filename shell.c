#include <stdio.h>
#include <string.h>
#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/errno.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/signal.h>
#include <stdlib.h>
#include <limits.h>
#include "shell.h"

void background(int signal){
    int temperrno = errno;
    pid_t pid;
    while((pid = waitpid(-1, NULL, WNOHANG)) > 0){
        printf("\npid %d done.\n", pid);
    }
    errno = temperrno;
}

void sigintcheck(int sign){
    signal(sign, SIG_IGN);
    fprintf(stderr, "\nError: Only exit can terminate the shell.\n");
}


int cd(char* path){
    char* token = "";
    char* last = "";
    char cwd[PATH_MAX];
    struct passwd *pw = getpwuid(getuid());
    if(pw == NULL){
        fprintf(stderr, "Error: Cannot get passwd entry. %s.\n", strerror(errno));
        return 1;
    }
    const char *home = pw->pw_dir;
    if(strcmp(path, "cd") == 0){
        chdir(home);
        return 0;
    }
    if(getcwd(cwd, 256 * sizeof(char)) == NULL){
        fprintf(stderr,"Error: Cannot get current working directory. %s.\n", strerror(errno));
        return 1;
    }

    //loop start
    token = strtok(path, "/");
    while(token != NULL){
    if(strcmp(token, "~") == 0){
        if(strcmp(last, "") != 0){ //~ cannot be called in a line
            fprintf(stderr, "Error: Cannot change directory to %s. %s.\n", path, strerror(errno));
            return 1;
        }
        chdir(home);
        strcpy(cwd, home);
        last = token;
        token = strtok(NULL, "/");
    } else {
    strcat(cwd, "/");
    strcat(cwd, token);
    if(chdir(cwd) == -1){
        fprintf(stderr, "Error: Cannot change directory to %s. %s.\n", path, strerror(errno));
        return 1;
    }   
        last = token;
        token = strtok(NULL, "/");
    }
}
    return 0;
}

int parse(char* argv[], int argc){
    //insert if-block checking what command it is
    if(strcmp(argv[0], "exit") == 0){
        if(argc >= 2){
            fprintf(stderr, "Error: Too many args for exit.\n");
            return 0;
        }
        return 2;
    }
    if(strcmp(argv[0], "cd") == 0){
        if(argc == 1){
            cd(argv[0]);
            return 0;
        }
        if(argc > 2){
            fprintf(stderr, "Error: Too many args for cd.\n");
            return 0;
        }
        if(strcmp(argv[argc -1], "&") == 0){
            fprintf(stderr, "Error: cd cannot run in background.\n");
        }
        return cd(argv[1]);
    }
    if(strcmp(argv[argc-1], "&") == 0){ //background process
    argv[argc-1] = '\0'; //remove &
    pid_t pid = fork();
    
    if(pid == -1){
        fprintf(stderr, "Error: fork() failed. %s.\n", strerror(errno));
        return 1;
    } else if(pid == 0){
        printf("pid: %d cmd: %s", getpid(), argv[0]);
        if((execvp(argv[0], argv) == -1)){
            fprintf(stderr, "Error: exec() failed. %s.\n", strerror(errno));
            return 1;
        }
        return 0;
    } else if(pid > 0){
        while((pid = waitpid(-1, NULL, WNOHANG)) > 0){
        printf("pid %d done.\n", pid);
    }
    }



    } else { //foreground process
    pid_t pid = fork(); 
    if(pid == -1){
        fprintf(stderr, "Error: fork() failed. %s.\n", strerror(errno));
        return 1;
    } else if(pid == 0){
        if((execvp(argv[0], argv) == -1)){
            fprintf(stderr, "Error: exec() failed. %s.\n", strerror(errno));
            return 1;
        }
        return 0;
    } else if(pid > 0){
        int wait;
        if(waitpid(pid, &wait, 0) == -1){
            fprintf(stderr, "Error: wait() failed. %s.\n", strerror(errno));
            return 1;
        }
        }
    }
    return EXIT_SUCCESS;
}

int main(int argc, char* argv[]){
    int MaxLine = 1024;
    char command[MaxLine];
    char* argvi[MaxLine];
    char* token = " ";
    int argci = 0;
    struct passwd *pwde;
    char* user;
    char cwd[PATH_MAX];
    if(signal(SIGINT, sigintcheck)==SIG_ERR){
        fprintf(stderr, "Error: Cannot register signal handler. %s.\n", strerror(errno));
    }
    if(signal(SIGCHLD, background) == SIG_ERR){
        fprintf(stderr, "Error: Cannot register signal handler. %s.\n", strerror(errno));
    }
    if((pwde = getpwuid(getuid())) == NULL){
        fprintf(stderr, "Error: Cannot get password entry. %s.\n", strerror(errno));
        return 1;
    }
    user = pwde->pw_name;
    if(getcwd(cwd, 256 * sizeof(char)) == NULL){
        fprintf(stderr,"Error: Cannot get current working directory. %s.\n", strerror(errno));
        return 1;
    }
    int parseresult = 0; //stores result of 'parse'
    while(1){
        if(getcwd(cwd, 256 * sizeof(char)) == NULL){
        fprintf(stderr,"Error: Cannot get current working directory. %s.\n", strerror(errno));
        return 1;
        }
        printf("Msh:%s:%s>", user, cwd);
        if ((fgets(command, MaxLine, stdin) == NULL) && ferror(stdin)){
            fprintf(stderr, "Error: Failed to read from stdin. %s.\n", strerror(errno));
        }
        if(strcmp(command, "\n") != 0){ //works for enter now

        command[strlen(command) -1] = '\0';
            if(!(command[0] == ' ')){ //if its just a space, dont do anything
            token = strtok(command, " ");
            while(token != NULL){
            argvi[argci] = token;
            argci++;
            token = strtok(NULL, " ");
            }
            parseresult = parse(argvi, argci);
            if(parseresult == 1){
                //invalid command
            } else if(parseresult == 2){
                //exit
                break;
            }
        }
        }
        //reset argc and argv
        argci = 0;
        memset(argvi, 0, MaxLine);
        //end of loop
    }
    if(strcmp(argvi[0], "exit") != 0){
    killpg(getpid(), SIGTERM);
    }
    return EXIT_SUCCESS;
}