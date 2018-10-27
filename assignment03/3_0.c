#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

/* FUNCTION PROTOTYPES */
void enter_n(int *n, int rank, MPI_Comm comm);
void init_matrix(int** matrix, int row, int col);
void gen_matrix_data(int *matrix, int *local_matrix, int n, int local_n, int rank, MPI_Comm comm);
void print_matrix(int *matrix, int row, int col, int rank);
void print_local_matrix(int *matrix, int row, int col, int rank);
void gen_vector_data(int* vector, int n, int rank, MPI_Comm comm);
void print_vector(int *vector, int n, int rank);
void solve_matrix_vec_mult(int* local_matrix, int* local_result, int* vector,int n, int local_n);
void print_local_result(int* local_result, int local_n, int n, int rank, MPI_Comm comm);
/* FUNCTION PROTOTYPES */

int main(int argc, char** argv){
    int n;
    int *matrix;
    int *vector;

    int local_n;
    int *local_matrix;
    int *local_result;

    int rank, comm_sz;
    MPI_Comm comm;

    MPI_Init(NULL,NULL);
    comm = MPI_COMM_WORLD;
    MPI_Comm_size(comm, &comm_sz);
    MPI_Comm_rank(comm, &rank);
    if(rank == 0)
         srand(time(0));

    enter_n(&n, rank, comm);

    local_n = n / comm_sz;

    vector = malloc(n * sizeof(int));
    matrix = malloc((n*n) * sizeof(int));

    local_result = malloc(local_n * sizeof(int));
    local_matrix = malloc((local_n*n) * sizeof(int));
    
    gen_matrix_data(matrix, local_matrix, n, local_n, rank, comm);
    gen_vector_data(vector, n, rank, comm);

    print_matrix(matrix, n, n, rank);
    print_vector(vector, n, rank);

    solve_matrix_vec_mult(local_matrix, local_result, vector, n, local_n);

    print_local_result(local_result, local_n, n, rank, comm);

    MPI_Finalize();
    return 0;
}

void enter_n(int *n, int rank, MPI_Comm comm){

    if(rank == 0){
        printf("Enter n:\n");
        scanf("%d", n);
        printf("\n");
    }
    MPI_Bcast(n, 1, MPI_INT, 0, comm);

} /* enter_n */

void init_matrix(int** matrix, int row, int col){
    for(int i = 0; i < row; i++){
        matrix[i] = malloc(col * sizeof(int));
    }
} /* init_matrix */

void solve_matrix_vec_mult(int* local_matrix, int* local_result, int* vector,int n, int local_n){
    for(int i = 0; i < local_n; i++){
        int start = i * n;
        int end = start + n;
        int sum = 0;
        // printf("start: %d\nend: %d\n", start, end);
        for(int j = start, k = 0; j < end && k < n; j++, k++){
            sum += local_matrix[j] * vector[k];
        }
        local_result[i] = sum;
    }
} /* solve_matrix_vec_mult */

void gen_matrix_data(int *matrix, int *local_matrix, int n, int local_n, int rank, MPI_Comm comm){

    if(rank == 0){
        // srand(time(0));
        int upper = 10;
        int lower = 1;
        int rand_num;
        for(int i = 0; i < (n*n); i++){
            rand_num = (rand() % (upper - lower + 1)) + lower; 
            matrix[i] = rand_num; 
        }
    }
    MPI_Bcast(matrix, n*n, MPI_INT, 0, comm);
    if(MPI_Scatter(matrix, local_n*n, MPI_INT, local_matrix, local_n*n, MPI_INT, 0, comm) != MPI_SUCCESS){
        perror("Mpi scatter: ");
        exit(1);
    }
} /* gen_matrix_data */

void gen_vector_data(int* vector, int n, int rank, MPI_Comm comm){
    if(rank == 0){
        // srand(time(0));
        int upper = 10;
        int lower = 1;
        int rand_num;
        for(int i = 0; i < n; i++){
            rand_num = (rand() % (upper - lower + 1)) + lower; 
            vector[i] = rand_num; 
        }
    }
    MPI_Bcast(vector, n, MPI_INT, 0, comm);
}

void print_matrix(int *matrix, int row, int col, int rank){
    if(rank == 0){
        printf("Matrix:\n");
        for(int i = 0; i < (row * col); i++){
            if(i % col == 0 && i != 0){
                printf("\n");
            }
            printf("%d ", matrix[i]);

        }
        printf("\n\n");
    }
} /* print_matrix */

void print_local_matrix(int *matrix, int row, int col, int rank){
        printf("Local matrix{%d}:\n", rank);
    // if(rank == 0){
        for(int i = 0; i < (row * col); i++){
            if(i % col == 0 && i != 0){
                printf("\n");
            }
            printf("%d ", matrix[i]);

        }
        printf("\n");
    // }
} /* print_local_matrix */

void print_vector(int *vector, int n, int rank){
    if(rank == 0){
        printf("Vector:\n");
        for(int i = 0; i < n; i++){
            printf("%d ", vector[i]);
        }
        printf("\n\n");
    }
} /* print_vector */

void print_local_result(int* local_result, int local_n, int n, int rank, MPI_Comm comm){
    
    int *a = NULL;
    if(rank == 0){
        a = malloc(n * sizeof(int));
        MPI_Gather(local_result, local_n, MPI_INT, a, local_n, MPI_INT, 0, comm);
        printf("Result:\n");
        for(int i = 0; i < n; i++){
            printf("%d ", a[i]);
        }
        printf("\n\n");
        free(a);
    }
    else{
        MPI_Gather(local_result, local_n, MPI_INT, a, local_n, MPI_INT, 0, comm);
    }

} /* print_local_result */