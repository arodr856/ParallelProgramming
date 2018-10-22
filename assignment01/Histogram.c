#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>
#include <time.h>

void userInput(int *data_count, float *min_val, float *max_val, int *bin_count);
void generateData(float data[], int data_count, float min_val, float max_val);
void printData(float data[], int data_count);
void printIntData(int data[], int data_count);
void setUpBins(float bin_max[], int bin_counts[], float bin_width, int bin_count, float min_val);
int findBin(float value, float bin_max[], int bin_count, int minMeas);

int main(int argc, char** argv){
   
    int data_count, bin_count;
    float min_val, max_val, bin_width;
    float *bin_max;
    int *bin_counts;


    userInput(&data_count, &min_val, &max_val, &bin_count);
    
    // init an array of size data_count
    float data[data_count];
    bin_max = malloc(bin_count * sizeof(float));
    bin_counts = malloc(bin_count * sizeof(int));
    bin_width = (max_val - min_val) / bin_count;
    
    // populate the data array with random floats
    generateData(data, data_count, min_val, max_val);
    setUpBins(bin_max, bin_counts, bin_width, bin_count, min_val);
    int binIndex;
    for(int i = 0; i < data_count; i++){
        binIndex = findBin(data[i], bin_max, bin_count, min_val);
        bin_counts[binIndex]++;
    }
    printf("Data:\n");
    printData(data, data_count);
    printf("Bin Max:\n");
    printData(bin_max, bin_count);
    printf("Bin Counts:\n");
    printIntData(bin_counts, bin_count);

    return 0;
}

void userInput(int *data_count, float *min_val, float *max_val, int *bin_count){

     // Ask for the amount of data to generate 
    printf("Enter the amount of data to generate:\n");
    scanf("%d", data_count);
    // Ask for the min_value
    printf("Enter the minimum value:\n");
    scanf("%f", min_val);
    // Ask for the max_value
    printf("Enter the max value:\n");
    scanf("%f", max_val);
    // Ask for the bin_count
    printf("Enter the number of bins to distribute the data:\n");
    scanf("%d", bin_count);

} /* userInput */

void generateData(float data[], int data_count, float min_val, float max_val){
    int i;
    float randomNum; 
    float range = (max_val - min_val);
    float div = RAND_MAX / range;

    srand(time(0));
    for(i = 0; i < data_count; i++){
        randomNum = min_val + (rand() / div);
        data[i] = randomNum;
    }
    
} /* generateDate */

void printData(float data[], int data_count){
    for(int i = 0; i < data_count; i++){
        printf("%f   ", data[i]);
    }
    printf("\n");
} /* printData */

void printIntData(int data[], int data_count){
    for(int i = 0; i < data_count; i++){
        printf("%d   ", data[i]);
    }
    printf("\n");
} /* printData */

void setUpBins(float bin_max[], int bin_counts[], float bin_width, int bin_count, float min_val){

    for(int i = 0; i < bin_count; i++){
        bin_max[i] = min_val + bin_width * (i + 1);
        bin_counts[i] = 0; 
    }

} /* setUpBins */

int findBin(float value, float bin_max[], int bin_count, int minMeas){

    for(int i = 0; i < bin_count; i++){
        if(value < bin_max[i]){
            return i;
        }
    }
    return -1;
}