/*****************************************************************************
* FILE: meanvar.c
* based on dotprod_mutex.c
* Credit: Robert Miller, Aman Khandelwal
* SOURCE: Vijay Sonnad, IBM
* dotprod_mutex.c LAST REVISED: 01/29/09 Blaise Barney
******************************************************************************/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

/* compile using -lpthread */


/*   
The following structure contains the necessary information  
to compute the mean and variance of a vector of doubles.
size is the total length of the vector
thread_len is the length to be processed by a single thread
*/

typedef struct 
 {
   double      *a;
   double     sum; 
   double    mean;
   double     var;
   int       size; 
   int thread_len;
 } meanvar;

/* Define globally accessible variables and a mutex */

#define NUMTHRDS 4
#define VECLEN 100000
   meanvar vector1; 
   pthread_t callThd[NUMTHRDS];
   pthread_mutex_t mutexsum;

void *variance(void *arg){

   int i, start, end, len ;
   long offset;
   double myvar, *b;
   offset = (long)arg;
  
  len = vector1.size/NUMTHRDS;
  start = offset * len;
  end = len + start;
  b = vector1.a;
  long squared = 0;
  
  for(i = start; i < end; i++){
    squared += b[i] * b[i];
  }
    squared -= vector1.mean;
    pthread_mutex_lock(&mutexsum);
    vector1.var += squared;
    pthread_mutex_unlock(&mutexsum);
    pthread_exit((void*) 0);
}
void *partial_sum(void *arg)
{

/* Define and use local variables for convenience */

   int i, start, end, len ;
   long offset;
   double mysum, *b;
   offset = (long)arg;
  
  len = vector1.size/NUMTHRDS;
  start = offset * len;
  end = len + start;
  b = vector1.a;
  
  for(i = start; i < end; i++){
    mysum += b[i];
  }
    pthread_mutex_lock(&mutexsum);
    vector1.sum += mysum;
    pthread_mutex_unlock(&mutexsum);
    pthread_exit((void*) 0);
}

/* 
The main program creates threads which do all the work and then 
print out result upon completion. Before creating the threads,
the input vector is created. Since all threads update a shared structure, we
need a mutex for mutual exclusion. The main thread needs to wait for
all threads to complete. We specify a thread attribute value that allow 
the main thread to join with the threads it creates. Note also that we free 
up handles  when they are no longer needed.
*/

int main (int argc, char *argv[])
{
long i;
double *a;
void *status;
pthread_attr_t attr;

/* Assign storage and initialize values */

a = (double*) malloc (VECLEN*sizeof(double));
  
for (i=0; i<VECLEN; i++) {
  a[i]=10.0*(rand()/(float)RAND_MAX);  
}

vector1.size = VECLEN; 
vector1.a = a; 
vector1.thread_len = (int) (VECLEN/NUMTHRDS);

/* TODO: initialize mutex */
pthread_mutex_init(&mutexsum, NULL);
/* TODO: Create threads to compute the mean  */
pthread_attr_init(&attr);
pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

for(i=0;i<NUMTHRDS;i++)
  {
  /* TODO: create threads to share the work */
   pthread_create(&callThd[i], &attr, partial_sum, (void* )i);
   }

pthread_attr_destroy(&attr);

/* TODO: Wait for the threads */
for(i = 0; i < NUMTHRDS; i++){
  pthread_join(callThd[i], &status);
}

/* TODO: compute mean, clean up */
long mean = vector1.sum / vector1.size;
vector1.mean = mean;
free(a);
pthread_mutex_destroy(&mutexsum);
pthread_exit(NULL);

/*     PHASE 2        */
/* TODO: compute the variance */


/*     PHASE 3        */
/* Can you compute the correlation coefficient of two vectors? */

return 0;
}   

