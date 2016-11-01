#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_N_INPUT 6000
#define MAX_M_INPUT 3000
#define MAX_P_INPUT 1000


int A[MAX_N_INPUT][MAX_M_INPUT];
int B[MAX_M_INPUT][MAX_P_INPUT];
int C[MAX_N_INPUT][MAX_P_INPUT];

//To access from thread initialized with mult_matrix
int *C_prime;

int m, n, p;

void * mult_matrix(int *);

int main(int argc, char *argv[] ){
  
  int num_threads = atoi(argv[1]);
  pthread tid[num_threads];
  int i, j;
  float start_time, end_time;
  printf("\nEnter n(<=6000), m(<=3000), p(<=1000): ");
  scanf(" %d %d %d", n, m, p);
  printf("\nYou entered: %d %d %d", n, m, p);
  
  //Initialize A and B values
  for(i = 0; i < n; i++){
    for(j = 0; j < m; j++){
      A[i][j] = i * j;
    }
  }
  
  for( i = 0; i < m; i++){
    for( j = 0; j < p; j++){
      B[i][j] = i + j;
    }
  }
  
  //Baseline
  start_time = gettimeofday();
  if( pthread_create(&tid[0], NULL, mult_matrix, &num_threads, 1, &C) < 0){
    printf("\nError Creating Thread. Terminating Program");
  }
  pthread_join(tid[0], NULL);
  end_time = gettimeofday();
  
  printf("\nThread\t\tSeconds");
  printf("\n1\t\t %.2f", (end_time - start_time));
  
  if(num_threads <= 1){
    return 0;
  }
  
  //Multi-Threading starting with 2 threads
  C_prime = malloc(sizeof(int) * n * p);
  int k, comp_error;
  
  for(i = 1; i < num_threads){
    start_time = gettimeofday();
    
    //create i threads to compute product
    for(j = 0; j < i; j++){
      int args[] = {i, j};
      if( pthread_create(&tid[j], NULL, mult_matrix, &args) < 0){
        printf("\nError Creating Thread. Terminating Program");
      }
    }
    //wait for all i threads to terminate
    for(j = 0; j < i; j++){
      pthread_join(tid[j],NULL)
    }
    end_time = gettimeofday();
    
    //compare C to C_prime
    comp_error = 0;
    for(j = 0; j < n; j ++){
      for(k = 0; k < p; k++){
        if(C[j][k] != C_prime[j][k]){
          comp_error = 1;
          break;
        }
      }
      if(comp_error){
        break;
      }
    }
    
    printf("/n%.2f\t\t", (end_time - start_time));
    printf("\ncomparison: ");
    if(comp_error){ printf("Error\n\n"); }
    else{ printf("No Error\n\n"); }
  }
  
  
  
}

/*
  *Multiplies 1/num_threads of Matricies A and B.
  *
  *@num_threads (args[0]: number of threads being used to compute the multiplication
  *@sequence (args[1]): the ordered number of the thread computing the product
  *   i.e. 1st thread, 2nd thread, ... , nth thread in a n-thread product
  *   calculation.
  *@result_matrix: The matrix to store the result in.
  *
  *In order to yield a proper product of matrices A and B, @num_threads threads must
  *   call this function such that for threads 1 to @num_threads, 
  *   thread 1 has sequence number 1, thread 2 has sequence number 2, etc.
*/
void * mult_matrix( int* args){
  int i, j, k;
  
  
  for(i = args[1]; i < n; i += args[0]){
    for( j = 0; j < p; j++){
      C_prime[i][j] = 0;
      
      for(k = 0; k < m; k++){
        C_prime[i][j] += A[i][k] * B[k][j]
      }
    }
  }
  
  pthread_exit(NULL)
}

















