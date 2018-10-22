#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>
#include <time.h>

void userInput(int *data_count, float *min_val, float *max_val, int *bin_count, int my_rank, int *local_data_count, MPI_Comm comm, int comm_sz);
void generateData(float data[], int data_count, float min_val, float max_val, int my_rank, MPI_Comm comm, int local_data_count, float local_data[]);
void printData(float data[], int data_count);
void printIntData(int data[], int data_count);
void setUpBins(float bin_max[], int local_data_count[], float bin_width, int bin_count, float min_val);
int findBin(float value, float bin_max[], int bin_count, int minMeas);
void printHist(float bin_max[], int bin_counts[], int bin_count);

int main(int argc, char** argv){
   
    int data_count, bin_count;
    float min_val, max_val, bin_width;
    float *bin_max;
    int *bin_counts;
    float *data;
    int local_data_count;
    int *local_bin_counts;
    float *local_data;
    int my_rank;
    int comm_sz;
    MPI_Comm comm;

    MPI_Init(NULL, NULL);
    comm = MPI_COMM_WORLD;
    MPI_Comm_rank(comm, &my_rank);
    MPI_Comm_size(comm, &comm_sz);

    userInput(&data_count, &min_val, &max_val, &bin_count, my_rank, &local_data_count, comm, comm_sz);

    // init an array of size data_count
    data = malloc(data_count * sizeof(float));
    local_data = malloc(local_data_count * sizeof(float));
    local_bin_counts = malloc(bin_count * sizeof(int));
    bin_max = malloc(bin_count * sizeof(float));
    bin_counts = malloc(bin_count * sizeof(int));
    bin_width = (max_val - min_val) / bin_count;

    // populate the data array with random floats
    generateData(data, data_count, min_val, max_val, my_rank, comm, local_data_count, local_data);
    setUpBins(bin_max, local_bin_counts, bin_width, bin_count, min_val);
    int binIndex;
    for(int i = 0; i < local_data_count; i++){
        binIndex = findBin(local_data[i], bin_max, bin_count, min_val);
        local_bin_counts[binIndex]++;
    }

    MPI_Reduce(local_bin_counts, bin_counts, bin_count, MPI_INT, MPI_SUM, 0, comm);
    
    if(my_rank == 0){
        printf("Data:\n");
        printData(data, data_count);
        printHist(bin_max, bin_counts, bin_count);
    }
    MPI_Finalize();
    return 0;
}

void printHist(float bin_max[], int bin_counts[], int bin_count){
    for(int i = 0; i < bin_count; i++){
        if(i == 0){
            printf("[0 , %f): %d\n", bin_max[i], bin_counts[i]);
        }else{
            printf("[%f , %f): %d\n",bin_max[i - 1],bin_max[i], bin_counts[i]);
        }
    }
    printf("\n");
}

void userInput(int *data_count, float *min_val, float *max_val, int *bin_count, int my_rank, int *local_data_count, MPI_Comm comm, int comm_sz){

    if(my_rank == 0){
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
        *local_data_count = *data_count / comm_sz;
    }
    MPI_Bcast(data_count, 1, MPI_INT, 0, comm);
    MPI_Bcast(min_val, 1, MPI_FLOAT, 0, comm);
    MPI_Bcast(max_val, 1, MPI_FLOAT, 0, comm);
    MPI_Bcast(bin_count, 1, MPI_INT, 0, comm);
    MPI_Bcast(local_data_count, 1, MPI_INT, 0, comm);
    
} /* userInput */

void generateData(float data[], int data_count, float min_val, float max_val, int my_rank, MPI_Comm comm, int local_data_count, float local_data[]){

    if(my_rank == 0){
        float randomNum; 
        float range = (max_val - min_val);
        float div = RAND_MAX / range;
        srand(time(0));
        int i;
        for(i = 0; i < data_count; i++){
            randomNum = min_val + (rand() / div);
            data[i] = randomNum;
        }
        MPI_Scatter(data, local_data_count, MPI_FLOAT, local_data, local_data_count, MPI_FLOAT, 0, comm);   
    }
    MPI_Bcast(data, data_count, MPI_FLOAT, 0, comm);
    MPI_Scatter(data, local_data_count, MPI_FLOAT, local_data, local_data_count, MPI_FLOAT, 0, comm);
    
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

void setUpBins(float bin_max[], int local_data_count[], float bin_width, int bin_count, float min_val){

    for(int i = 0; i < bin_count; i++){
        bin_max[i] = min_val + bin_width * (i + 1);
        local_data_count[i] = 0; 
    }

} /* setUpBins */

int findBin(float value, float bin_max[], int bin_count, int minMeas){

    for(int i = 0; i < bin_count; i++){
        if(value < bin_max[i]){
            return i;
        }
    }
    return -1;
} /* findBin */