//Robert Miller
//I pledge my Honor that I have abided by the Stevens Honor System
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stddef.h>
#include "mergesort.h"
//used my 385 for a template
int int_cmp(const void* a, const void* b){
    if(*(int*)a == *(int*)b){
        return 0;
    }
    if(*(int*)a > *(int*)b){
    return 1;
    }
    //a < b
    return -1;
}
int dbl_cmp(const void* a, const void* b){
    if(*(double*)a == *(double*)b){
        return 0;
    }
    if(*(double*)a > *(double*)b){
        return 1;
    }
    //a < b
    return -1;
}
//helpers

void merger(void* array, int l, int m, int r, size_t len, size_t elem_sz, int (*comp)(const void*, const void*)){
    int i, j, k;

    if(elem_sz == sizeof(double)){
        double* res = (double*) array; //resultant array pointer
        double* temp = (double*) malloc(len * sizeof(double));
        for(i = l, j = m+1, k = l; i <= m && j <= r; k++){
            if(comp((char*)array+(i* elem_sz), (char*)array+(j * elem_sz)) <= 0){
                temp[k] = res[i++];
            } else {
                temp[k] = res[j++];
            }
        }
        while(i <= m){
            temp[k++] = res[i++];
        }
        while(j <= r){
            temp[k++] = res[j++];
        }

        for(k = l; k <=r ; k++){
            res[k] = temp[k];
        }
        free(temp);
    } else {
        int *res = (int*) array; //resultant array pointer
        int* temp = (int*) malloc(len * sizeof(int));
        for(i = l, j = m+1, k = l; i <= m && j <= r; k++){
            if(comp((char*)array+(i* elem_sz), (char*)array+(j * elem_sz)) <= 0){
                temp[k] = res[i++];
            } else {
                temp[k] = res[j++];
            }
        }
        while(i <= m){
            temp[k++] = res[i++];
        }
        while(j <= r){
            temp[k++] = res[j++];
        }

        for(k = l; k <=r ; k++){
            res[k] = temp[k];
        }
        free(temp);
    }
}


void sort(void* array, size_t len, size_t elem_sz, int low, int high, int (*comp)(const void*, const void*)){
int mid;
if(low < high){
    mid = (low + high)/2;
    sort(array, len, elem_sz, low, mid, comp);
    sort(array, len, elem_sz, mid + 1, high, comp);
    merger(array, low, mid, high, len, elem_sz, comp);
    } else {
    return;
    }
}
void mergesort(void*  array, size_t len, size_t elem_sz, int (*comp)(const void*, const void*)){
sort(array, len, elem_sz, 0, len-1, comp);
}