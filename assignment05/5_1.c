#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

/* FUNCTION PROTOTYPES */
void enter_n(int *n);
void init_rows(int** matrix, int n);
void print_matrix(int **matrix, int n);
void gen_matrix_data(int **matrix, int n);
void gen_vector_data(int* vector, int n);
void print_vector(int *vector, int n);
void* solve_matrix_mult(void* rank);
/* FUNCTION PROTOTYPES */

int n;
int thread_n;
int *final_result;
int *vector;
int **matrix;


int main(int argc, char **argv){
    srand(time(NULL));

    int thread_count= atoi(argv[1]);
    pthread_t *threads;

    enter_n(&n);
    // printf("n: %d\n", n);
    thread_n = n / thread_count;
    vector = malloc(n * sizeof(int));
    matrix = malloc(n * sizeof(int*));
    final_result = malloc(n * sizeof(int));
    threads = malloc(thread_count * sizeof(pthread_t));

    init_rows(matrix, n);
    // print_matrix(matrix, n);

    gen_matrix_data(matrix, n);
    print_matrix(matrix, n);

    gen_vector_data(vector, n);
    print_vector(vector, n);

    for(int i = 0; i < thread_count; i++){
        pthread_create(&threads[i], NULL, solve_matrix_mult, (void*) i);
    }

    for(int i = 0; i < thread_count; i++){
        pthread_join(threads[i], NULL);
    }

    print_vector(final_result, n);
    free(vector);
    free(matrix);
    free(final_result);
    free(threads);
    return 0;
} /* main */

void enter_n(int *n){
    printf("enter n: ");
    scanf("%d", n);
    printf("\n");
} /* enter_n */

void init_rows(int** matrix, int n){
    for(int i = 0; i < n; i++){
        matrix[i] = malloc(n * sizeof(int));
    }
} /* init_rows */

void print_matrix(int **matrix, int n){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
} /* print_matrix */

void gen_matrix_data(int **matrix, int n){
    int upper = 10;
    int lower = 1;
    int rand_num;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            rand_num = (rand() % (upper - lower + 1)) + lower; 
            matrix[i][j] = rand_num; 
        }
    }
} /* gen_matrix_data */

void gen_vector_data(int* vector, int n){

    int upper = 10;
    int lower = 1;
    int rand_num;
    for(int i = 0; i < n; i++){
        rand_num = (rand() % (upper - lower + 1)) + lower; 
        vector[i] = rand_num; 
    }
} /* gen_vector_data */

void print_vector(int *vector, int n){
    printf("Vector:\n");
    for(int i = 0; i < n; i++){
        printf("%d ", vector[i]);
    }
    printf("\n\n");
} /* print_vector */

void* solve_matrix_mult(void* rank){
    int r = (int) rank;
    int start = r * thread_n;
    int end = start + thread_n;
    int sum;
    for(int row = start; row < end; row++){
        sum = 0;
        for(int col = 0; col < n; col++){
            sum += (matrix[row][col] * vector[col]);
        }
        final_result[row] = sum;
    }
    return NULL;
} /* solve_matrix_mult */