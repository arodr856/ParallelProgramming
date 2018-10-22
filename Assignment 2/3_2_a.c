#include <stdio.h>
/*
 * Serial program to compute the prefix sum of an array of ints
 */
#include <stdlib.h>
#include <time.h>

void populate_array(int* array, int n);
int* prefix_sum(int* array, int n);
void Read_n(int* n);
void print_array(int* array, int n);

int main(int argc, char** argv){

    int n;
    int* array;
    
    Read_n(&n);

    array = malloc(n * sizeof(int));
    populate_array(array, n);
    
    print_array(array, n);
   
    int* prefix = prefix_sum(array, n);
    print_array(prefix, n);

    return 0;
}

void Read_n(int* n){
        printf("Enter n:\n");
        scanf("%d", n);
}

void populate_array(int* array, int n){

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

int* prefix_sum(int* array, int n){

        int* result = malloc(n * sizeof(int));
        result[0] = array[0];
        for(int i = 1; i < n; i++){
            result[i] = array[i] + result[i - 1];
        }
        return result; 
}

void print_array(int* array, int n){
    for(int i = 0; i < n; i++){
        printf("%d ", array[i]);
    }
    printf("\n");
}