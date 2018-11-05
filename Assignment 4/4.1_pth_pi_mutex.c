  #include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include "timer.h"

const int MAX_THREADS = 1024;

long thread_count;
long long n;
double sum = 0;
pthread_mutex_t mutex;

void* Thread_sum(void* rank);
double generate_num();

/* Only executed by main thread */
void Get_args(int argc, char* argv[]);
void Usage(char* prog_name);
double Serial_pi(long long n);

int main(int argc, char* argv[]) {
   long       thread;  /* Use long in case of a 64-bit system */
   pthread_t* thread_handles;
   double start, finish, elapsed;

   /* Get number of threads from command line */
   Get_args(argc, argv);

   if(pthread_mutex_init(&mutex, NULL) != 0){
      printf("\n mutex init has failed\n"); 
      return 1; 
   }

   thread_handles = malloc(thread_count * sizeof(pthread_t));
   int thread_n = n / thread_count;

   for(int i = 0; i < thread_count; i++){
     pthread_create(thread_handles+i, NULL, Thread_sum, (void *)thread_n);
   }

  for(int i = 0; i < thread_count; i++){
    pthread_join(thread_handles[i], NULL);
  }

  printf("sum: %lf\n", sum);
  double pi_estimate = (4 * sum) / ((double) n);
  printf("pi estimate: %lf\n", pi_estimate);
  pthread_mutex_destroy(&mutex);
  return 0;
}  /* main */

/*------------------------------------------------------------------*/
void* Thread_sum(void* rank) {
    // printf("Number of thread tosses: %d\n", (int) rank);
    for(int toss = 0; toss < (int) rank; toss++){
      double x = generate_num();
      double y = generate_num();
      double distance_squared = (x*x) + (y*y);
      if(distance_squared <= 1){
        pthread_mutex_lock(&mutex);
        sum++;
        pthread_mutex_unlock(&mutex);
      }  
    }

   return NULL;
}  /* Thread_sum */

double generate_num(){
	return (double)rand()/RAND_MAX*2.0-1.0;
} /* generate_num */

/*------------------------------------------------------------------
 * Function:   Serial_pi
 * Purpose:    Estimate pi using 1 thread
 * In arg:     n
 * Return val: Estimate of pi using n terms of Maclaurin series
 */
double Serial_pi(long long n){
   double sum = 0.0;
   long long i;
   double factor = 1.0;

   for (i = 0; i < n; i++, factor = -factor) {
      sum += factor/(2*i+1);
   }
   return 4.0*sum;

}  /* Serial_pi */

/*------------------------------------------------------------------
 * Function:    Get_args
 * Purpose:     Get the command line args
 * In args:     argc, argv
 * Globals out: thread_count, n
 */
void Get_args(int argc, char* argv[]) {
   if (argc != 3) Usage(argv[0]);
   thread_count = strtol(argv[1], NULL, 10);  
   if (thread_count <= 0 || thread_count > MAX_THREADS) Usage(argv[0]);
   n = strtoll(argv[2], NULL, 10);
   if (n <= 0) Usage(argv[0]);
}  /* Get_args */


/*------------------------------------------------------------------
 * Function:  Usage
 * Purpose:   Print a message explaining how to run the program
 * In arg:    prog_name
 */
void Usage(char* prog_name) {
   fprintf(stderr, "usage: %s <number of threads> <n>\n", prog_name);
   fprintf(stderr, "   n is the number of terms and should be >= 1\n");
   fprintf(stderr, "   n should be evenly divisible by the number of threads\n");
   exit(0);
}  /* Usage */
