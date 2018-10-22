/*
 *
 * parallel program that computes the prefix sum for an array that equals the size of the number of procs
 */ 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

void populate_array(int* array, int n, int my_rank, MPI_Comm comm);
int* prefix_sum(int* array, int n);
void Read_n(int* n);
void print_array(int* array, int n, int my_rank, char title[]);

int main(int argc, char** argv){

    int n;
    int* array;
    int process_sum = 0, process_value;
    
    int my_rank, comm_sz;
    MPI_Comm comm;

    // Read_n(&n);
    MPI_Init(NULL, NULL);
    comm = MPI_COMM_WORLD;
    MPI_Comm_rank(comm, &my_rank);
    MPI_Comm_size(comm, &comm_sz);
    n = comm_sz;

    array = malloc(n * sizeof(int));
    populate_array(array, n, my_rank, comm);
    print_array(array, n, my_rank, "Array:");
    process_value = array[my_rank];
    MPI_Scan(&process_value, &process_sum, 1, MPI_INT, MPI_SUM, comm);

    printf("process %d prefix sum: %d\n",my_rank, process_sum);

    free(array);
    MPI_Finalize();
    return 0;
}

void Read_n(int* n){
        printf("Enter n:\n");
        scanf("%d", n);
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

int* prefix_sum(int* array, int n){

        int* result = malloc(n * sizeof(int));
        result[0] = array[0];
        for(int i = 1; i < n; i++){
            result[i] = array[i] + result[i - 1];
        }
        return result; 
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