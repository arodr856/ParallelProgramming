#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

/* FUNCTION PROTOTYPES */
void enter_not(int *num_of_tosses);
void* get_num_in_circle(void* value);
double generate_num();
void estimate_pi(double *pi_estimate, int num_in_circle, int num_of_tosses);
/* FUNCTION PROTOTYPES */

pthread_mutex_t mutex;

int num_of_tosses = 0, num_in_circle = 0;

int main(int argc, char **argv){
    
    pthread_t *threads;
    int thread_count, thread_num_of_tosses;
    double pi_estimate;

    if(argc == 2){
        thread_count = atoi(argv[1]);
        threads = malloc(thread_count * sizeof(pthread_t));
    }else{
        printf("Error: Run Format: ./5_3 <thread_count>\n");
        exit(0);
    }

    enter_not(&num_of_tosses);
    
    thread_num_of_tosses = num_of_tosses / thread_count;

    pthread_mutex_init(&mutex, NULL);

    for(int i = 0; i < thread_count; i++){
        pthread_create(&threads[i], NULL, get_num_in_circle, (void*)thread_num_of_tosses);
    }


    for(int i = 0; i < thread_count; i++){
        pthread_join(threads[i], NULL);
    }

    estimate_pi(&pi_estimate, num_in_circle, num_of_tosses);
    printf("Num of tosses: %d\n", num_of_tosses);
    printf("Num in circle: %d\n", num_in_circle);
    printf("pi estimate: %f\n", pi_estimate);

    pthread_mutex_destroy(&mutex);
    return 0;
} /* main */

void enter_not(int *num_of_tosses){
    printf("Enter the number of tosses: ");
    scanf("%d", num_of_tosses);
    printf("\n");
} /* enter_not */

void* get_num_in_circle(void* value){
    int tosses = (int) value;
    double x, y, distance_sqaured; 
    for(int toss = 0; toss < tosses; toss++){
        x = generate_num(); 
        y = generate_num();
        distance_sqaured = (x*x) + (y*y);
        if(distance_sqaured <= 1){
            pthread_mutex_lock(&mutex);
            num_in_circle++;
            pthread_mutex_unlock(&mutex);
        }
    }
    return NULL;
} /* get_num_in_circle */

double generate_num(){
	return (double)rand()/RAND_MAX*2.0-1.0;
} /* generate_num */

void estimate_pi(double *pi_estimate, int num_in_circle, int num_of_tosses){
	*pi_estimate = (4 * num_in_circle) / ((double) num_of_tosses);
} /* estimate_pi */
