/*
 * Parallel program that computes the prefix sum of an array using a collective communication (MPI_scan)
 * 
 */ 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

void populate_array(int* array, int n, int my_rank, MPI_Comm comm);
void prefix_sum(int* array, int* local_prefix_sum, int n);
void Read_n(int* n, int my_rank, MPI_Comm comm);
void print_array(int* array, int n, int my_rank, char title[]);
void print_local_array(int* array, int local_n);
void sum_array(int* array, int* local_array_sum, int local_n);
void setup_local_prefix_sum(int* local_array, int* local_prefix_sum, int sum, int local_n);

int main(int argc, char** argv){

    int n;
    int sum = 0;
    int* array;
    int* prefix_sum_array;

    int local_n, local_array_sum;
    int* local_array;
    int* local_prefix_sum;

    int my_rank, comm_sz;
    MPI_Comm comm;

    MPI_Init(NULL, NULL);
    comm = MPI_COMM_WORLD;
    MPI_Comm_rank(comm, &my_rank);
    MPI_Comm_size(comm, &comm_sz);

    Read_n(&n, my_rank, comm);
    local_n = (n / comm_sz);

    array = malloc(n * sizeof(int));
    prefix_sum_array = malloc(n * sizeof(int));
    local_array = malloc(local_n * sizeof(int));
    local_prefix_sum = malloc(local_n * sizeof(int));

    /* Populate the array and scatter them to the local array */
    populate_array(array, n, my_rank, comm);
    print_array(array, n, my_rank, "Array"); // print populated array
    MPI_Scatter(array, local_n, MPI_INT, local_array, local_n, MPI_INT, 0, comm);

    sum_array(local_array, &local_array_sum, local_n);
    MPI_Scan(&local_array_sum, &sum, 1, MPI_INT, MPI_SUM, comm);
    setup_local_prefix_sum(local_array, local_prefix_sum, sum, local_n);

    print_local_array(local_prefix_sum, local_n);

    MPI_Finalize();
    return 0;
}

void setup_local_prefix_sum(int* local_array, int* local_prefix_sum, int sum, int local_n){
    
    local_prefix_sum[local_n - 1] = sum;
    for(int i = local_n - 2; i >= 0; i--){
        local_prefix_sum[i] = local_prefix_sum[i + 1] - local_array[i + 1];
    }

}

void sum_array(int* array, int* local_array_sum, int local_n){
    *local_array_sum = 0;
    for(int i = 0; i < local_n; i++){
        *local_array_sum += array[i];
    }
}

void Read_n(int* n, int my_rank, MPI_Comm comm){
    if(my_rank == 0){
        printf("Enter n:\n");
        scanf("%d", n);
    }
    MPI_Bcast(n, 1, MPI_INT, 0, comm);
}

void populate_array(int* array, int n, int my_rank, MPI_Comm comm){
    if(my_rank == 0){
        int min = 0, max = 10;
        int randomNum; 
        int range = (max - min);
        int div = RAND_MAX / range;

        srand(time(0));

        for(int i = 0; i < n; i++){
            randomNum = min + (rand() / div);
            array[i] = randomNum;
        }
    }
    MPI_Bcast(array, n, MPI_INT, 0, comm);
}

void prefix_sum(int* array, int* local_prefix_sum, int n){

       
        local_prefix_sum[0] = array[0];
        for(int i = 1; i < n; i++){
            local_prefix_sum[i] = array[i] + local_prefix_sum[i - 1];
        }
       
}

void print_array(int* array, int n, int my_rank, char title[]){
    if(my_rank == 0){
        printf("%s: ", title);
        for(int i = 0; i < n; i++){
            printf("%d ", array[i]);
        }
        printf("\n");
    }
}

void print_local_array(int* array, int local_n){
    for(int i = 0; i < local_n; i++){
            printf("%d ", array[i]);
        }
        printf("\n");
}