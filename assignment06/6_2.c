/**
 * Program is ran in the following manner:
 * compile: gcc -Wall -o ./bin/6_2 6_2.c
 * Run: ./bin/6_2 <number of threads> <number of tasks>
**/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


struct ListNode{
  int data;
  struct ListNode* next;
};

// FUNCTION PROTOTYPES
int Member(int value, struct ListNode* head_p);
int Insert(int value, struct ListNode** head_p);
int Delete(int value, struct ListNode** head_p);
void printLinkedList(struct ListNode* head_p);
void generateSortedLinkedList();
void* perform_mult_task(void* task_count);
// FUNCTION PROTOTYPES

struct ListNode* linked_list;
pthread_rwlock_t lock_rw;

int main(int argc, char **argv){
  srand(time(0));

  int thread_count = atoi(argv[1]);
  int task_count = atoi(argv[2]);
  pthread_t *threads;
  threads = malloc(thread_count * sizeof(pthread_t));

  generateSortedLinkedList();
  printf("Original Sorted LinkedList:\n");
  printLinkedList(linked_list);

  pthread_rwlock_init(&lock_rw, NULL);
  for(int task = 0; task < thread_count; task++){
    pthread_create(&threads[task], NULL, perform_mult_task, (void*) task_count);
  }

  for(int task = 0; task < thread_count; task++){
    pthread_join(threads[task], NULL);
  }  
  
  printf("Final linked list:\n");
  printLinkedList(linked_list);
  pthread_rwlock_destroy(&lock_rw);
  free(threads);

  return 0;
} /* main */

int Member(int value, struct ListNode* head_p){

  struct ListNode* curr_p = head_p;
  
  while(curr_p != NULL && curr_p -> data < value)
    curr_p = curr_p -> next;
  
  if(curr_p == NULL || curr_p -> data > value)
    return 0;
  else
    return 1;

} /* Member */

int Insert(int value, struct ListNode** head_p){

  struct ListNode* curr_p = *head_p;
  struct ListNode* pred_p = NULL;
  struct ListNode* temp_p;

  while(curr_p != NULL && curr_p -> data < value){
    pred_p = curr_p;
    curr_p = curr_p -> next;
  }

  if(curr_p == NULL || curr_p -> data > value){
    temp_p = malloc(sizeof (struct ListNode));
    temp_p -> data = value;
    temp_p -> next = curr_p;

    if(pred_p == NULL){
      *head_p = temp_p;
    }else{
      pred_p -> next = temp_p;
    }
     return 1;
  }
  return 0;
} /* Insert */

int Delete(int value, struct ListNode** head_p){
  struct ListNode* curr_p = *head_p;
  struct ListNode* pred_p = NULL;

  while(curr_p != NULL && curr_p -> data < value){
    pred_p = curr_p;
    curr_p = curr_p -> next;
  }

  if(curr_p != NULL && curr_p -> data == value){
    if(pred_p == NULL){ // Deleting the first node in the list
      *head_p = curr_p -> next;
      free(curr_p);
    }else{
      pred_p -> next = curr_p -> next;
      free(curr_p);
    }
    return 1;
  }

  return 0; // value isnt in list
} /* Delete */

void printLinkedList(struct ListNode* head_p){
  struct ListNode* curr_p = head_p;
  if(curr_p -> next == NULL){
     printf("%d ", curr_p -> data);
  }else{
    while(curr_p != NULL){
      printf("%d ", curr_p -> data);
      curr_p = curr_p -> next;
    }
  }
  printf("\n");
} /* printLinkedList */

void generateSortedLinkedList(){
  int list_count = 0;
  while(list_count < 10){
    int randomNumber = rand() % 100 + 1;
    if(!Member(randomNumber, linked_list)){
      Insert(randomNumber, &linked_list);
      list_count++;
    }
  }
} /* generateSortedLinkedList */

void* perform_mult_task(void* task_count){

  int t_count = (int) task_count;

  /**
   * tasks:
   * 0: Member
   * 1: Insert
   * 2: Delete
  */

  for(int task = 0; task < t_count; task++){
    int task_number =(rand() %  (2 - 0 + 1)) + 0; 
    int randomNumber = rand() % 100 + 1;
    switch(task_number){
      case 0:
        pthread_rwlock_rdlock(&lock_rw);
        printf("executing member function. . .\n");
        int hasValue = Member(randomNumber, linked_list);
        if(hasValue)
          printf("LinkedList contains value: %d\n\n", randomNumber);
        else
          printf("LinkedList does not contain value: %d\n\n", randomNumber);
        pthread_rwlock_unlock(&lock_rw);
        break;
      case 1:
        pthread_rwlock_wrlock(&lock_rw);
        printf("executing insert function. . .\n");
        int valueInserted = Insert(randomNumber, &linked_list);
        if(valueInserted){
          printf("%d was inserted into linked list. . .\n", randomNumber);
          printLinkedList(linked_list);
          printf("\n");
        }  
        else
          printf("%d already existed in the linked list. . .\n\n", randomNumber);
        pthread_rwlock_unlock(&lock_rw);
        break;
      case 2:
        pthread_rwlock_wrlock(&lock_rw);
        printf("executing delete function. . .\n");
        int deleted = Delete(randomNumber, &linked_list);
        if(deleted){
          printf("%d was deleted from list. . .\n\n", randomNumber);
          printLinkedList(linked_list);
          printf("\n");
        }  
        else
          printf("%d did not exist in linked list. . .\n\n", randomNumber);
        pthread_rwlock_unlock(&lock_rw);
        break;
      default:
        printf("Number did not match any task. . .\n\n");
    }
  }

  return NULL;
} /* perform_mult_task */

