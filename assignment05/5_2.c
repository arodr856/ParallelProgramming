#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>


/* FUNCTION PROTOTYPES */
void userInput(int *data_count, int *bin_count, float *min_val, float *max_val);
void generateData(float *data, int data_count, float min_val, float max_val);
void setUpBins(float *bin_max, int *bin_counts, float bin_width, int bin_count, float min_val);
void printFloatArray(float *array, int count);
void* thread_findBin(void *rank);
int findBin(float value, float *bin_max, int bin_count);
void printHist(float *bin_max, int *bin_counts, int bin_count, float min_val);
/* FUNCTION PROTOTYPES */



int data_count, bin_count, thread_data_count, thread_count;
float min_val, max_val, bin_width;
float *data;
float *bin_max;
int *bin_counts; 
pthread_mutex_t mutex;


int main(int argc, char **argv){

    pthread_t *threads;

    srand(time(NULL));
    thread_count = atoi(argv[1]);
    userInput(&data_count, &bin_count, &min_val, &max_val);
    // printf("data_count: %d\nbin_count: %d\nmin_val: %f\nmax_val: %f\n", data_count, bin_count, min_val, max_val);    

    thread_data_count = data_count / thread_count;
    threads = malloc(thread_count * sizeof(pthread_t));
    data = malloc(data_count * sizeof(float));
    bin_max = malloc(bin_count * sizeof(float));
    bin_counts = malloc(bin_count * sizeof(int));
    bin_width = (max_val - min_val) / bin_count;

    generateData(data, data_count, min_val, max_val);
    printf("\nDATA:\n");
    printFloatArray(data, data_count);

    setUpBins(bin_max, bin_counts, bin_width, bin_count, min_val);
    printf("MAX BINS:\n");
    printFloatArray(bin_max, bin_count);

    pthread_mutex_init(&mutex, NULL);
    for(int i = 0; i < thread_count; i++){
        pthread_create(&threads[i], NULL, thread_findBin, (void*) i);
    }
    
    
    for(int i = 0; i < thread_count; i++){
        pthread_join(threads[i], NULL);
    }

    printHist(bin_max, bin_counts, bin_count, min_val);
    pthread_mutex_destroy(&mutex);
    return 0;
} /* main */

void userInput(int *data_count, int *bin_count, float *min_val, float *max_val){
    // Ask for the amount of data to generate 
    printf("Enter the amount of data to generate: ");
    scanf("%d", data_count);
    // Ask for the min_value
    printf("Enter the minimum value: ");
    scanf("%f", min_val);
    // Ask for the max_value
    printf("Enter the max value: ");
    scanf("%f", max_val);
    // Ask for the bin_count
    printf("Enter the number of bins to distribute the data: ");
    scanf("%d", bin_count);
} /* userInput */

void generateData(float *data, int data_count, float min_val, float max_val){
    float randomNum; 
    float range = (max_val - min_val);
    float div = RAND_MAX / range;
    int i;
    for(i = 0; i < data_count; i++){
        randomNum = min_val + (rand() / div);
        data[i] = randomNum;
    }

} /* generateDate */

void setUpBins(float *bin_max, int *bin_counts, float bin_width, int bin_count, float min_val){

    for(int i = 0; i < bin_count; i++){
        bin_max[i] = min_val + bin_width * (i + 1);
        bin_counts[i] = 0; 
    }

} /* setUpBins */

void printFloatArray(float *array, int count){

    for(int i = 0; i < count; i++){
        printf("%f ", array[i]);
    }
    printf("\n\n");
} /* printFloatArray */

void* thread_findBin(void *rank){
    int r = (int)rank;
    int start = r * (thread_data_count);
    int end = start + thread_data_count;
    int binIndex;
    for(int i = start; i < end; i++){
        binIndex = findBin(data[i], bin_max, bin_count);
        pthread_mutex_lock(&mutex);
        bin_counts[binIndex]++; // critical section
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int findBin(float value, float *bin_max, int bin_count){
    for(int i = 0; i < bin_count; i++){
        if(value < bin_max[i]){
            return i;
        }
    }
    return -1;
} /* findBin */

void printHist(float *bin_max, int *bin_counts, int bin_count, float min_val){
    for(int i = 0; i < bin_count; i++){
        if(i == 0){
            printf("[%f , %f): %d\n",min_val, bin_max[i], bin_counts[i]);
        }else{
            printf("[%f , %f): %d\n",bin_max[i - 1],bin_max[i], bin_counts[i]);
        }
    }
    printf("\n");
} /* printHist */
