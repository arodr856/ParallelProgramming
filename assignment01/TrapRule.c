#include <stdlib.h>
#include <stdio.h>
#include<mpi.h>

double f(double x);
double Trap(double left_endpt, double right_endpt, int trap_count, double base_len);
void userInput(double *a, double *b, int *n, int my_rank, MPI_Comm comm);


int main(int argc, char **argv){

    int my_rank, comm_sz, n, local_n;
    double a, b, h, local_a, local_b;
    double local_int, total_int;
    
    MPI_Comm comm;

    MPI_Init(NULL, NULL);
    comm = MPI_COMM_WORLD;
    MPI_Comm_rank(comm, &my_rank);
    MPI_Comm_size(comm, &comm_sz);

    userInput(&a, &b, &n, my_rank, comm);

    h = (b - a) / n;
    local_n = n / comm_sz;

    local_a = a + (my_rank * local_n * h); 
    local_b = local_a + (local_n * h);
    local_int = Trap(local_a, local_b, local_n, h);

    MPI_Reduce(&local_int, &total_int, 1,MPI_DOUBLE, MPI_SUM, 0, comm);
    if(my_rank == 0){
        printf("with n = %d trapezoids, our estimate\nof the integral from %f to %f = %.15e\n", n, a,b,total_int);
    }

    MPI_Finalize();
    return 0;
}

void userInput(double *a, double *b, int *n, int my_rank, MPI_Comm comm){
    if(my_rank == 0){
        printf("Enter a value for a:\n");
        scanf("%lf", a);
        printf("Enter a value for b:\n");
        scanf("%lf", b);
        printf("Enter a value for n:\n");
        scanf("%d", n);
    }
    MPI_Bcast(a, 1, MPI_DOUBLE, 0, comm);
    MPI_Bcast(b, 1, MPI_DOUBLE, 0, comm);
    MPI_Bcast(n, 1, MPI_INT, 0, comm);
}

double Trap(
        double left_endpt,
        double right_endpt,
        int trap_count,
        double base_len){
    
    double sum_of_traps, x;
    int i;
    
    sum_of_traps = (f(left_endpt) + f(right_endpt)) / 2.0;
    for(i = 1; i <= trap_count - 1; i++){
        x = left_endpt + i*base_len;
        sum_of_traps += f(x);
    }
    sum_of_traps *= base_len;
    return sum_of_traps;

}

double f(double x){
    return x * x * x;
}