#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>


void enter_not(int *num_of_tosses, int rank, MPI_Comm comm);
int get_num_in_circle(int local_num_of_tosses);
double generate_num();
void estimate_pi(double *pi_estimate, int num_in_circle, int num_of_tosses);

int main(int argc, char** argv){
        // comment
	int num_of_tosses, num_in_circle;

	int local_num_in_circle, local_num_of_tosses;
	double pi_estimate;

	int rank, comm_sz;

	MPI_Comm comm;

	MPI_Init(NULL, NULL);
	comm = MPI_COMM_WORLD;
	MPI_Comm_rank(comm, &rank);
	MPI_Comm_size(comm, &comm_sz);
	if(rank == 0)
         srand(time(0));

	enter_not(&num_of_tosses, rank, comm);

	local_num_of_tosses = num_of_tosses / comm_sz;
	
	local_num_in_circle = get_num_in_circle(local_num_of_tosses);

	MPI_Reduce(&local_num_in_circle, &num_in_circle, 1, MPI_INT, MPI_SUM, 0, comm);
	
	if(rank == 0){
		estimate_pi(&pi_estimate, num_in_circle, num_of_tosses);
		printf("pi estimate: %f\n", pi_estimate);
	}

   
	MPI_Finalize();
    return 0;
}

void enter_not(int *num_of_tosses, int rank, MPI_Comm comm){
	if(rank == 0){
		printf("Enter the number of tosses: \n");
		scanf("%d", num_of_tosses);
		printf("\n");
	}
	MPI_Bcast(num_of_tosses, 1, MPI_INT, 0, comm);
}


void estimate_pi(double *pi_estimate, int num_in_circle, int num_of_tosses){
	*pi_estimate = (4 * num_in_circle) / ((double) num_of_tosses);
}

int get_num_in_circle(int local_num_of_tosses){
	int number_in_circle = 0;
	for(int toss = 0; toss < local_num_of_tosses; toss++){
		double x = generate_num();
		double y = generate_num();
		double distance_squared = (x*x) + (y*y);
		if(distance_squared <= 1)
			number_in_circle++;
	}
	return number_in_circle;
}

double generate_num(){
	return (double)rand()/RAND_MAX*2.0-1.0;
}

