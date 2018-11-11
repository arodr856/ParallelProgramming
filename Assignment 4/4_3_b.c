#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
const int MAX_REPS = 100;

void* produce_consume(void* rank);
void remove_first_item();

pthread_mutex_t mutex;
int message_available = 0;
char *messages[10];


int main(int argc, char **argv){
    // srand(0);

    pthread_t *threads;
    for(int i = 0; i < 10; i++){
        messages[i] = malloc(20 * sizeof(char));
    }

    int k = atoi(argv[1]);
    int thread_count = 2 * k;
    // int thread_count = 2;
    if(pthread_mutex_init(&mutex, NULL) != 0){
        printf("\n mutex init has failed\n"); 
        return 1; 
    }
     
    // allocate memory for multiple threads
    threads = malloc(thread_count * sizeof(pthread_t));

    for(int i = 0; i < thread_count; i++){
        pthread_create(&threads[i], NULL, produce_consume, (void*)i);
    }



    for(int i = 0; i < thread_count; i++){
        pthread_join(threads[i], NULL);
    }

    return 0;
} /* main */

void* produce_consume(void* rank){
    int r = (int) rank;

    while(1){

        pthread_mutex_lock(&mutex);
        if(r % 2 != 0){ // consumer
            if(message_available){
                printf("Th %d > %s\n", r, messages[0]);
                remove_first_item();
                message_available--;
                pthread_mutex_unlock(&mutex);
                break;
            }

        } else{ // producer
            char *str;
            str = malloc(20 * sizeof(char));
            sprintf(str, "hello from %d\n", r);
            messages[message_available] = str;
            // printf("hello from  %d. . .\n", rand_num);
            message_available++;    
            pthread_mutex_unlock(&mutex);
            break;
        }
        pthread_mutex_unlock(&mutex);
    } /* end of while loop */

    return NULL;
} /* produce_consume */

void remove_first_item(){
    for(int i = 0; i < message_available; i++){
        messages[i] = messages[i + 1];
    }
}



