/*
 * parallel program to computer the vector sum of two vectors
*/
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void Read_n(int* vector_size, int* local_vector_size, int my_rank, int comm_sz, MPI_Comm comm);
void Read_vector(int* local_x_vector, int* local_y_vector, int vector_size, int local_vector_size, int my_rank, MPI_Comm comm);
void Print_vector(int* local_vector, int local_vector_size,int vector_size, int my_rank, MPI_Comm comm, char title[]);
void Add_local_vectors(int* local_x_vector, int* local_y_vector, int local_vector_size, int* local_vector_sum);

int main(int argc, char** argv){

    int vector_size;
    int comm_sz;
    int my_rank;

    int* local_x_vector;
    int* local_y_vector;
    int local_vector_size;
    int* local_vector_sum;
    MPI_Comm comm;

    MPI_Init(NULL, NULL);
    comm = MPI_COMM_WORLD;
    MPI_Comm_rank(comm, &my_rank);
    MPI_Comm_size(comm, &comm_sz);

    Read_n(&vector_size, &local_vector_size, my_rank, comm_sz, comm);

    local_x_vector = malloc(local_vector_size * sizeof(int));
    local_y_vector = malloc(local_vector_size * sizeof(int));
    local_vector_sum = malloc(local_vector_size * sizeof(int));

    Read_vector(local_x_vector, local_y_vector, vector_size, local_vector_size, my_rank, comm);

    Add_local_vectors(local_x_vector, local_y_vector, local_vector_size, local_vector_sum);
    
    Print_vector(local_x_vector, local_vector_size, vector_size, my_rank, comm, "Vector X:");
    Print_vector(local_y_vector, local_vector_size, vector_size, my_rank, comm, "Vector Y:");
    Print_vector(local_vector_sum, local_vector_size, vector_size, my_rank, comm, "Vector Sum:");
    
    MPI_Finalize(); 
    return 0;
}

void Add_local_vectors(int* local_x_vector, int* local_y_vector, int local_vector_size, int* local_vector_sum){
    for(int i = 0; i < local_vector_size; i++){
        local_vector_sum[i] = local_x_vector[i] + local_y_vector[i];
    }
}

void Read_n(int* vector_size, int* local_vector_size, int my_rank, int comm_sz, MPI_Comm comm){
    if(my_rank == 0){
        printf("What is size of the vectors:\n");
        scanf("%d", vector_size);  
        *local_vector_size = *vector_size / comm_sz; 
    }
    MPI_Bcast(vector_size, 1, MPI_INT, 0, comm);
    MPI_Bcast(local_vector_size, 1, MPI_INT, 0, comm);
} /* Read_n */

void Read_vector(int* local_x_vector, int* local_y_vector, int vector_size, int local_vector_size, int my_rank, MPI_Comm comm){

    int* x_vector = malloc(vector_size * sizeof(int));
    int* y_vector = malloc(vector_size * sizeof(int));            

    if(my_rank == 0){
        printf("Enter %d values for vector x:\n", vector_size);
        for(int i = 0; i < vector_size; i++){
            scanf("%d", &x_vector[i]);
        }
        printf("Enter %d values for vector y:\n", vector_size);
        for(int i = 0; i < vector_size; i++){
            scanf("%d", &y_vector[i]);
        }
    }
    MPI_Scatter(x_vector, local_vector_size, MPI_INT, local_x_vector, local_vector_size, MPI_INT, 0, comm);
    MPI_Scatter(y_vector, local_vector_size, MPI_INT, local_y_vector, local_vector_size, MPI_INT, 0, comm);
    free(x_vector);
    free(y_vector);
} /* Read_vector */

void Print_vector(int* local_vector, int local_vector_size,int vector_size, int my_rank, MPI_Comm comm, char title[]){
    
    int* vector = NULL;

    if(my_rank == 0){
        vector = malloc(vector_size * sizeof(int));
        MPI_Gather(local_vector, local_vector_size, MPI_INT, vector, local_vector_size, MPI_INT, 0, comm);
        printf("%s\n", title);
        for(int i = 0; i < vector_size; i++){
            printf("%d ", vector[i]);
        }
        printf("\n");
    }else{
        MPI_Gather(local_vector, local_vector_size, MPI_INT, vector, local_vector_size, MPI_INT, 0, comm);
    }
    
} /* Print_vector */


