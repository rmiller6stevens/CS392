#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <getopt.h>
#include "mergesort.h"

//please ignore all the prinf error checking, its been a long night <3

void usage(){
    printf("Usage: ./sort [-i|-d] filename\n-i: Specifies the file contains ints.\n-d: Specifies the file contains doubles.\nfilename: The file to sort.\n");
}

int main(int argc, char *argv[]) {
    FILE *fp;
    char *copy;
    int opt;
    int ic = 0; //counter for # of i's
    int dc = 0; //counter for # of d's
    int check = 0; //these are basically booleans, so im using them as such
    int checkmorefiles = 0;
    while((opt = getopt (argc, argv, ":id")) != -1){
        switch(opt){
            case 'i': //i
            ic++; //for future referece, opt is the filename
            //printf("%s, %s\n", argv[optind], argv[optind+1]);
            if(argv[optind] != NULL && argv[optind + 1] != NULL){
                //printf("%s\n", argv[optind]);
                //printf("YES\n");
                checkmorefiles = 1;
            } else if(argv[optind] == NULL){
                check = 1;
            } else {
            //printf("%s\n", optarg);
            //copy = argv[optind];
        
            copy = (char*) malloc(strlen(argv[optind]) * sizeof(char));
            strcpy(copy, argv[optind]);
            
            fp = fopen(copy, "r");
            }
            break;

            case 'd': //d
            dc++; //for future referece, opt is the filename
            //printf("%s, %s\n", argv[optind], argv[optind+1]);
            if(argv[optind] != NULL && argv[optind + 1] != NULL){
                //printf("%s\n", argv[optind]);
                //printf("YES\n");
                checkmorefiles = 1;
            } else if(argv[optind] == NULL){
                check = 1;
            } else {
            //copy = argv[optind];
            
            copy = (char*) malloc(strlen(argv[optind]) * sizeof(char));
            strcpy(copy, argv[optind]);
            
            fp = fopen(copy, "r");
            }
            break;
            case '?': //unknown
            printf("Error: unkown option '-%c' recieved.\n", optopt);
            usage();
            return EXIT_FAILURE;
        }
    }
//check here for too many flags
// || !strcmp(copy, "d") || !strcmp(copy, "i")
if(ic + dc > 1 ){
    fprintf(stderr, "Error: Too many flags specified.\n");
    return EXIT_FAILURE;
}
//check for too many files
if(checkmorefiles){
    fprintf(stderr, "Error: Too many files specified.\n");
    return EXIT_FAILURE;
}
//check for no filenames passed
if(check){
    fprintf(stderr, "Error: No input file specified.\n");
    return EXIT_FAILURE;
}
//check for no args
if(argc == 1){ 
    usage();
    return EXIT_FAILURE;
}
//check here for if filenames are not existing
if(fp == NULL){
    fprintf(stderr, "Error: Cannot open '%s'. %s.\n", copy, strerror(errno));
    free(copy);
    return EXIT_FAILURE;
}
//OK FINALLY PAST THE ERROR PARSING
//line counter
int lc = 0;
char c;
for(c = getc(fp); c != EOF; c = getc(fp)){
    if(c == '\n'){
        lc++;
    }
}
if(ic){
    //do the mersort on int
    fclose(fp);
    fp = fopen(copy, "r");
    int* arr = (int*) malloc(lc * sizeof(int));
    for(int i = 0; i < lc; i++){
        fscanf(fp, "%d", &arr[i]);
    }
    mergesort(arr, lc, sizeof(int), &int_cmp);
    //print here
    for(int i = 0; i < lc; i++){
        printf("%d\n", arr[i]);
    }
    free(arr);
} else if (dc){
    double* arr = (double*) malloc(lc * sizeof(double));
    fclose(fp);
    fp = fopen(copy, "r");
    for(int i = 0; i < lc; i++){
        fscanf(fp, "%lf", &arr[i]);
        //printf("Should be in file %lf\n", arr[i]);
    }
    mergesort(arr, lc, sizeof(double), dbl_cmp);
    
    for(int i = 0; i < lc; i++){
        printf("%lf\n", arr[i]);
    }
    
    //do the mergesort on doubles
free(arr);
}

free(copy);
fclose(fp);
return EXIT_SUCCESS;
}