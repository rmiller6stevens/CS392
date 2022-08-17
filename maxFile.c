#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>

void func1(char* input, size_t *nus, size_t *nnus, char* nu, char* nnu, size_t *total, int dont_print){ //is now fun2
    struct dirent *file;
    struct stat currfile;
    char path[256];
    DIR* dir = opendir(input);
    while((file = readdir(dir)) != NULL){
        strcpy(path, input);
        strcat(path, "/");
        strcat(path, file->d_name); //full path name made
        if(stat(path, &currfile) != -1){
            if(S_ISREG(currfile.st_mode)){ //if its a file  WHEN IT IS DIRECTORY, UPDATE 
            //SET INPUT TO PATH AND RUN IT AGAIN RECURSIVELY
                if(currfile.st_mode & S_IWUSR){ //if the file user can write
                    if(currfile.st_size >= *nus){
                        *nus = currfile.st_size;
                        strcpy(nu, file->d_name);
                     }
                        *total += currfile.st_size;
                    } else {
                    if(currfile.st_size >= *nnus){
                        *nnus = currfile.st_size;
                        strcpy(nnu, file->d_name);
                    }
                        *total += currfile.st_size;
                }
            } else if(S_ISDIR(currfile.st_mode)){
                if(strcmp(file->d_name,".") && strcmp(file->d_name, "..")){
                    size_t temp = *total;
                    *total = 0; //do this so it prints out size of directory being looked at
                func1(path, nus, nnus, nu, nnu, total, 1);
                *total = *total + 4096;
                printf("%ld\t%s\n", *total, path);
                *total = *total + temp;//readd total
                }
            }
		} else {
            if(strcmp(file->d_name,".") && strcmp(file->d_name, "..")){
            fprintf(stderr, "'%s' cannot be accessed because %s.\n", file->d_name, strerror(errno));
            }
        }
    }
    if(dont_print == 0){ //wont print if subdirectory
    *total += 4096;
    printf("%ld\t%s\n", *total, input);
    }
    closedir(dir);
}
int main(int argc, char *argv[]) {
    if(argc != 2){
        printf("Usage: %s <directory>.\n", argv[0]);
        return EXIT_FAILURE;
    }
        DIR* dir = opendir(argv[1]);
    if (dir == NULL){
        closedir(dir);
        fprintf(stderr, "Error: Cannot open '%s'. %s.\n", argv[1], strerror(errno));
        return EXIT_FAILURE;
    }
    closedir(dir);
    char nnu[50] = "No file found";
    char nu[50] = "No file found";
    size_t nus = 0; //user write size
    size_t nnus = 0; //user not write size
    size_t total = 0;
    func1(argv[1], &nus, &nnus, nu, nnu, &total, 0);
    printf("User cannot write largest file '%s', has size %zu.\nLargest file where user can write is '%s', and has size %zu.\n", nnu, nnus, nu, nus);
    return EXIT_SUCCESS;
}
