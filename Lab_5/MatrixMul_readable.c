#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define MAX_N_INPUT 6000
#define MAX_M_INPUT 3000
#define MAX_P_INPUT 1000

//Used to pass multiple arguments to pthread_create start-up process
struct arg_struc {
  int num_threads;
  int seq_num;
  int **result_matrix;
};

//Global varibles accessed by threaded processes
int **A;
int **B;
int **C;
int **C_prime;
int m, n, p;

//Public functions
void * mult_matrix(void *arg_struc);
void clear_matrix(int **matrix, int rows, int cols);

int main(int argc, char *argv[] ){
  
  int num_threads = atoi(argv[1]);
  pthread_t tid[num_threads];
  int i, j;
  struct timeval start_time, end_time;
  
  //input loop
  do{
    printf("\nEnter n(<=6000), m(<=3000), p(<=1000): ");
    scanf(" %d %d %d", &n, &m, &p);
    printf("\nYou entered: %d %d %d", n, m, p);
  }while(n > MAX_N_INPUT && m > MAX_M_INPUT &&  p > MAX_P_INPUT);
  
  A = (int **)malloc(n * sizeof(int *));
  B = (int **)malloc(m * sizeof(int *));
  C = (int **)malloc(n * sizeof(int *));
  C_prime = (int **)malloc(n * sizeof(int *));
  
  //Initialize A and B values
  for(i = 0; i < n; i++){
    A[i] = (int *)malloc(m * sizeof(int));
    for(j = 0; j < m; j++){
      A[i][j] = i * j;
    }
  }
  
  //Debug Message
  printf("\nInitialized A and B");
  
  for( i = 0; i < m; i++){
    B[i] = (int *)malloc(p * sizeof(int));
    for( j = 0; j < p; j++){
      B[i][j] = i + j;
    }
  }
  
  //Finish allocating memory for C and C_prime without initialization
  for( i = 0; i < n; i++){
    C[i] = (int *)malloc( p * sizeof(int));
    C_prime[i] = (int *)malloc( p * sizeof(int));
  }
  
  //Debug Message
  printf("\nAllocated memory for C and C_prime");
  
  //Baseline
  gettimeofday(&start_time, NULL);
  struct arg_struc *baseline = malloc(sizeof(struct arg_struc));
  
  baseline->num_threads = 1;
  baseline->seq_num = 0;
  baseline->result_matrix = C;
  //struct arg_struc baseline = {.num_threads = 1, .seq_num = 0, .result_matrix = C};
  if( pthread_create(&tid[0], NULL, mult_matrix, (void *)baseline) < 0){
    printf("\nError Creating Thread. Terminating Program");
    return -1;
  }
  
  //Debug Message
  printf("\n---Waiting for baseline computation---");
  
  pthread_join(tid[0], NULL);
  gettimeofday(&end_time, NULL);
  
  printf("\nThread\t\tSeconds");
  printf("\n1\t\t %ld", (end_time.tv_sec - start_time.tv_sec));
  
  if(num_threads <= 1){
    return 0;
  }
  
  //Multi-Threading starting with 2 threads
  int k, comp_error;
  struct arg_struc *thread_args = malloc(sizeof(struct arg_struc) * num_threads);
  
  for(i = 1; i < num_threads; i++){
    gettimeofday(&start_time, NULL);
    
    //create i threads to compute product
    for(j = 0; j < i; j++){
      //struct arg_struc args = {.num_threads = i + 1,  .seq_num = j, .result_matrix = C_prime};
      thread_args[j].num_threads = i + 1;
      thread_args[j].seq_num = j;
      thread_args[j].result_matrix = C_prime;
      
      if( pthread_create(&tid[j], NULL, &mult_matrix, (void *)&thread_args[j] ) < 0){
        printf("\nError Creating Thread. Terminating Program");
        return -1;
      }
    }
    //wait for all i threads to terminate
    for(j = 0; j < i; j++){
      pthread_join(tid[j],NULL);
    }
    gettimeofday(&end_time, NULL);
    
    //compare C to C_prime
    comp_error = 0;
    for(j = 0; j < n; j ++){
      for(k = 0; k < p; k++){
        if( C[j][k] != C_prime[j][k]){
          comp_error = 1;
          break;
        }
      }
      if(comp_error){
        break;
      }
    }
    
    printf("\n%d\t\t%ld", (i+1), (end_time.tv_sec - start_time.tv_sec));
    printf("\ncomparison: ");
    if(comp_error){ printf("Error\n\n"); }
    else{ printf("No Error\n\n"); }
  }
  
  return 0;
  
}

/*
  *Multiplies 1/num_threads of Matricies A and B.
  *
  *@num_threads: number of threads being used to compute the multiplication
  *@sequence: the ordered number of the thread computing the product
  *   i.e. 1st thread, 2nd thread, ... , nth thread in a n-thread product
  *   calculation.
  *@result_matrix: The matrix to store the result in.
  *
  *In order to yield a proper product of matrices A and B, @num_threads threads must
  *   call this function such that for threads 1 to @num_threads, 
  *   thread 1 has sequence number 1, thread 2 has sequence number 2, etc.
*/
void * mult_matrix( void *arguments){
  int i, j, k;
  
  //Debug Message
  printf("\nBefore parsing args from pthread_create");
  printf("\narguments: %d", arguments);
  printf("\nAfter trying to print arguments");
  
  //Debug Message
  printf("\nsizeof(arguments): %d", sizeof((struct arg_struc*)arguments));
  printf("\nsizeof(struct arg_struc *): %d", sizeof(struct arg_struc*));
  
  //TODO: problem statement
  struct arg_struc *args = (struct arg_struc *)arguments;
  
  //Debug Message
  printf("\nAfter assigning args within new thread");
  
  //Debug Message -- Creating Seg Fault
  printf("\nCreating Thread [%d]", args->seq_num);
  
  
  for(i = args->seq_num; i < n; i += args->num_threads){
    for( j = 0; j < p; j++){
      args->result_matrix[i][j] = 0;
      
      for(k = 0; k < m; k++){
        args->result_matrix[i][j] += A[i][k] * B[k][j];
      }
    }
  }
  
  pthread_exit(NULL);
}

void clear_matrix(int **matrix, int rows, int cols){
  int i, j;
  
  for(i = 0; i < rows; i++){
    for(j = 0; j < cols; j++){
      matrix[i][j] = 0;
    }
  }
  
}















