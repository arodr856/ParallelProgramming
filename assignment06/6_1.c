#include <stdlib.h>
#include <stdio.h>
#include<mpi.h>
#include<pthread.h>

double f(double x);
void* Trap(void * data);
void userInput(double *a, double *b, int *n);


struct Data{
    double a;
    double h;
    double thread_n;
    int rank;
};

double trap_sums = 0.0;

pthread_mutex_t mutex;

int main(int argc, char **argv){


    int thread_count = atoi(argv[1]);
    int n, thread_n = 0;
    double a, b, h;
    pthread_t *threads;

    userInput(&a, &b, &n);
    threads = malloc(thread_n * sizeof(pthread_t));

    h = (b - a) / n;
    thread_n = n / thread_count;

    pthread_mutex_init(&mutex, NULL);

    for(int i = 0; i < thread_count; i++){
        struct Data *d = (struct Data*)malloc(sizeof *d);
        d->a = a;
        d->h = h;
        d->thread_n = thread_n;
        d->rank = i;
        pthread_create(&threads[i], NULL, Trap, (void*) d);
    }

    for(int i = 0; i < thread_count; i++){
        pthread_join(threads[i], NULL);
    }

    printf("with n = %d trapezoids, our estimate\nof the integral from %f to %f = %.15e\n", n, a,b,trap_sums);

    pthread_mutex_destroy(&mutex);
    return 0;
}

void userInput(double *a, double *b, int *n){
        printf("Enter a value for a:\n");
        scanf("%lf", a);
        printf("Enter a value for b:\n");
        scanf("%lf", b);
        printf("Enter a value for n:\n");
        scanf("%d", n);
}

void* Trap(void* data){
    struct Data d = *(struct Data*) data;
    // printf("a: %f", )
    double thread_a  = d.a + (d.rank * d.thread_n * d.h); 
    double thread_b = thread_a + (d.thread_n * d.h);

    double sum_of_traps, x;

    sum_of_traps = (f(thread_a) + f(thread_b)) / 2.0;
    for(int i = 1; i <= d.thread_n - 1; i++){
        x = thread_a + i*d.h;
        sum_of_traps += f(x);
    }
    sum_of_traps *= d.h;
    pthread_mutex_lock(&mutex);
    trap_sums += sum_of_traps;
    pthread_mutex_unlock(&mutex);
    return NULL;

}

double f(double x){
    return x * x * x;
}