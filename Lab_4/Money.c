#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>

/* Prototypes for thread procedures */
void *thread_1();
void *thread_2();
void *thread_3();
void *thread_4();

pthread_t tid[4];
sem_t mutex, sync_1, sync_2, sync_3, sync_4;

int accounts[3] = {100000, 100000, 100000};

int main(){
  int return_val;
  
  /*--- Initialization of semaphores and threads ---*/
  return_val = sem_init(&mutex, 0, 1);
  if(return_val < 0){
    printf("Failed to open semaphore: mutex");
    return -1;
  }
  return_val = sem_init(&sync_1, 0, 1);
    if(return_val < 0){
    printf("Failed to open semaphore: sync_1");
    return -1;
  }
  return_val = sem_init(&sync_2, 0, 1);
    if(return_val < 0){
    printf("Failed to open semaphore: sync_2");
    return -1;
  }
  return_val = sem_init(&sync_3, 0, 1);
    if(return_val < 0){
    printf("Failed to open semaphore: sync_3");
    return -1;
  }
  return_val = sem_init(&sync_4, 0, 1);
    if(return_val < 0){
    printf("Failed to open semaphore: sync_4");
    return -1;
  }
  
  return_val = pthread_create(&tid[0], NULL, thread_1, NULL);
  if(return_val < 0){
    printf("Failed to create thread_1");
    return -1;
  }
  return_val = pthread_create(&tid[1], NULL, thread_2, NULL);
  if(return_val < 0){
    printf("Failed to create thread_2");
    return -1;
  }
  return_val = pthread_create(&tid[2], NULL, thread_3, NULL);
  if(return_val < 0){
    printf("Failed to create thread_3");
    return -1;
  }
  
  /*--- Wait for thread_2 to terminate ---*/
  
  pthread_join(tid[1], NULL);
  
  sem_post(&sync_4);
  
  //pthread_exit(NULL);
  return 0;
}

void *thread_1(){
  int i, internal_reg;
  i = pthread_create(&tid[3], NULL, thread_4, NULL);
  if(i < 0){
    printf("Failed to create thread_4");
    pthread_exit(NULL);
  }
  
  /*--- 4-way sync ---*/
  if(sem_post(&sync_2) < 0){ printf("\nthread_1: sem_post error sync_2"); pthread_exit(NULL);}
  if(sem_post(&sync_3) < 0){ printf("\nthread_1: sem_post error sync_3"); pthread_exit(NULL);}
  if(sem_post(&sync_4) < 0){ printf("\nthread_1: sem_post error sync_4"); pthread_exit(NULL);}
  
  if(sem_wait(&sync_1) < 0){ printf("\nthread_1: sem_wait error 1"); pthread_exit(NULL);}
  if(sem_wait(&sync_1) < 0){ printf("\nthread_1: sem_wait error 2"); pthread_exit(NULL);}
  if(sem_wait(&sync_1) < 0){ printf("\nthread_1: sem_wait error 3"); pthread_exit(NULL);}
  
  for(i = 0; i < 10000; i++){
    
    if( (i % 3000) == 0 && i != 0){
      usleep(200000);
    }
    
    if(sem_wait(&mutex) < 0){ printf("\nthread_1: sem_wait mutex error"); pthread_exit(NULL);};
    
    internal_reg = accounts[0];
    internal_reg -= 100;
    accounts[0] = internal_reg;
    
    internal_reg = accounts[1];
    internal_reg += 100;
    accounts[1] = internal_reg;
    
    if(sem_post(&mutex) < 0){ printf("\nthread_1: sem_post mutex error"); pthread_exit(NULL);};
    
    if( (i % 1000 == 0) && (i != 0) ){
      int acc_1_val, acc_2_val, acc_3_val;
      
      if(sem_wait(&mutex) < 0){ printf("\nthread_1: sem_wait mutex error"); pthread_exit(NULL);};
      acc_1_val = accounts[0];
      acc_2_val = accounts[1];
      acc_3_val = accounts[2];
      if(sem_post(&mutex) < 0){ printf("\nthread_1: sem_post mutex error"); pthread_exit(NULL);};
      
      printf("\n|Thread_1| account_1: %d  account_2: %d  account_3: %d  sum: %d",
                acc_1_val, acc_2_val, acc_3_val, ( acc_1_val + acc_2_val + acc_3_val ));
    }
  }
  
  int acc_1_val, acc_2_val, acc_3_val;

  if(sem_wait(&mutex) < 0){ printf("\nthread_3: sem_wait mutex error"); pthread_exit(NULL);};
  acc_1_val = accounts[0];
  acc_2_val = accounts[1];
  acc_3_val = accounts[2];
  if(sem_post(&mutex) < 0){ printf("\nthread_3: sem_post mutex error"); pthread_exit(NULL);};

  printf("\n|Thread_3| account_1: %d  account_2: %d  account_3: %d  sum: %d",
  acc_1_val, acc_2_val, acc_3_val, ( acc_1_val + acc_2_val + acc_3_val ));
  
  sem_post(&sync_4);
  
  pthread_exit(NULL);
}

void *thread_2(){
  int i, internal_reg;
  
  /*--- 4-way sync ---*/
  if(sem_post(&sync_1) < 0){ printf("\nthread_2: sem_post error sync_1"); pthread_exit(NULL);}
  if(sem_post(&sync_3) < 0){ printf("\nthread_2: sem_post error sync_3"); pthread_exit(NULL);}
  if(sem_post(&sync_4) < 0){ printf("\nthread_2: sem_post error sync_4"); pthread_exit(NULL);}
  
  if(sem_wait(&sync_2) < 0){ printf("\nthread_2: sem_wait error 1"); pthread_exit(NULL);}
  if(sem_wait(&sync_2) < 0){ printf("\nthread_2: sem_wait error 2"); pthread_exit(NULL);}
  if(sem_wait(&sync_2) < 0){ printf("\nthread_2: sem_wait error 3"); pthread_exit(NULL);}
  
  for(i = 0; i < 10000; i++){
    if( i == 2000 || i == 4000 || i == 6000 || i == 9000){
      usleep(200000);
    }
    
    if(sem_wait(&mutex) < 0){ printf("\nthread_2: sem_wait mutex error"); pthread_exit(NULL);};
    
    internal_reg = accounts[1];
    internal_reg -= 100;
    accounts[1] = internal_reg;
    
    internal_reg = accounts[2];
    internal_reg += 100;
    accounts[2] = internal_reg;
    
    if(sem_post(&mutex) < 0){ printf("\nthread_2: sem_post mutex error"); pthread_exit(NULL);};
    
    if( (i % 1000 == 0) && (i != 0) ){
      int acc_1_val, acc_2_val, acc_3_val;
      
      if(sem_wait(&mutex) < 0){ printf("\nthread_2: sem_wait mutex error"); pthread_exit(NULL);};
      acc_1_val = accounts[0];
      acc_2_val = accounts[1];
      acc_3_val = accounts[2];
      if(sem_post(&mutex) < 0){ printf("\nthread_2: sem_post mutex error"); pthread_exit(NULL);};
      
      printf("\n|Thread_2| account_1: %d  account_2: %d  account_3: %d  sum: %d",
                acc_1_val, acc_2_val, acc_3_val, ( acc_1_val + acc_2_val + acc_3_val ));
    }
  }
  
  int acc_1_val, acc_2_val, acc_3_val;

  if(sem_wait(&mutex) < 0){ printf("\nthread_3: sem_wait mutex error"); pthread_exit(NULL);};
  acc_1_val = accounts[0];
  acc_2_val = accounts[1];
  acc_3_val = accounts[2];
  if(sem_post(&mutex) < 0){ printf("\nthread_3: sem_post mutex error"); pthread_exit(NULL);};

  printf("\n|Thread_3| account_1: %d  account_2: %d  account_3: %d  sum: %d",
  acc_1_val, acc_2_val, acc_3_val, ( acc_1_val + acc_2_val + acc_3_val ));
  
  sem_post(&sync_4);
  
  pthread_exit(NULL);
}

void *thread_3(){
  int i, internal_reg;
  
  /*--- 4-way sync ---*/
  if(sem_post(&sync_2) < 0){ printf("\nthread_3: sem_post error sync_2"); pthread_exit(NULL);}
  if(sem_post(&sync_1) < 0){ printf("\nthread_3: sem_post error sync_1"); pthread_exit(NULL);}
  if(sem_post(&sync_4) < 0){ printf("\nthread_3: sem_post error sync_4"); pthread_exit(NULL);}
  
  if(sem_wait(&sync_3) < 0){ printf("\nthread_3: sem_wait error 1"); pthread_exit(NULL);}
  if(sem_wait(&sync_3) < 0){ printf("\nthread_3: sem_wait error 2"); pthread_exit(NULL);}
  if(sem_wait(&sync_3) < 0){ printf("\nthread_3: sem_wait error 3"); pthread_exit(NULL);}
  
  for(i = 0; i < 10000; i++){
    
    if( i == 2000 || i == 7500){
      usleep(300000);
    }
    
    if(sem_wait(&mutex) < 0){ printf("\nthread_3: sem_wait mutex error"); pthread_exit(NULL);};
    
    internal_reg = accounts[2];
    internal_reg -= 100;
    accounts[2] = internal_reg;
    
    internal_reg = accounts[0];
    internal_reg += 100;
    accounts[0] = internal_reg;
    
    if(sem_post(&mutex) < 0){ printf("\nthread_3: sem_post mutex error"); pthread_exit(NULL);};
    
    if( (i % 1000 == 0) && (i != 0) ){
      int acc_1_val, acc_2_val, acc_3_val;
      
      if(sem_wait(&mutex) < 0){ printf("\nthread_3: sem_wait mutex error"); pthread_exit(NULL);};
      acc_1_val = accounts[0];
      acc_2_val = accounts[1];
      acc_3_val = accounts[2];
      if(sem_post(&mutex) < 0){ printf("\nthread_3: sem_post mutex error"); pthread_exit(NULL);};
      
      printf("\n|Thread_3| account_1: %d  account_2: %d  account_3: %d  sum: %d",
                acc_1_val, acc_2_val, acc_3_val, ( acc_1_val + acc_2_val + acc_3_val ));
    }
  }
  
  int acc_1_val, acc_2_val, acc_3_val;

  if(sem_wait(&mutex) < 0){ printf("\nthread_3: sem_wait mutex error"); pthread_exit(NULL);};
  acc_1_val = accounts[0];
  acc_2_val = accounts[1];
  acc_3_val = accounts[2];
  if(sem_post(&mutex) < 0){ printf("\nthread_3: sem_post mutex error"); pthread_exit(NULL);};

  printf("\n|Thread_3| account_1: %d  account_2: %d  account_3: %d  sum: %d",
  acc_1_val, acc_2_val, acc_3_val, ( acc_1_val + acc_2_val + acc_3_val ));
  
  sem_post(&sync_4);
  
  pthread_exit(NULL);
}

void *thread_4(){
  int i, sum_wrong = 0, acc_1_val, acc_2_val, acc_3_val;
  
  /*--- 4-way sync ---*/
  if(sem_post(&sync_2) < 0){ printf("\nthread_4: sem_post error sync_2"); pthread_exit(NULL);}
  if(sem_post(&sync_1) < 0){ printf("\nthread_4: sem_post error sync_1"); pthread_exit(NULL);}
  if(sem_post(&sync_3) < 0){ printf("\nthread_4: sem_post error sync_3"); pthread_exit(NULL);}
  
  if(sem_wait(&sync_4) < 0){ printf("\nthread_4: sem_wait error 1"); pthread_exit(NULL);}
  if(sem_wait(&sync_4) < 0){ printf("\nthread_4: sem_wait error 2"); pthread_exit(NULL);}
  if(sem_wait(&sync_4) < 0){ printf("\nthread_4: sem_wait error 3"); pthread_exit(NULL);}
  
  for(i = 0; i < 50000; i++){
    if( (i % 10000) == 0 && i != 0){
      usleep(50000);
    }
    
    if(sem_wait(&mutex) < 0){ printf("\nthread_4: sem_wait mutex error"); pthread_exit(NULL);};
    acc_1_val = accounts[0];
    acc_2_val = accounts[1];
    acc_3_val = accounts[2];
    if(sem_post(&mutex) < 0){ printf("\nthread_4: sem_post mutex error"); pthread_exit(NULL);};
    
    if( (acc_1_val + acc_2_val + acc_3_val) != 300000){
      sum_wrong++;
    }
  }
  
  sem_wait(&sync_4);
  sem_wait(&sync_4);
  sem_wait(&sync_4);
  sem_wait(&sync_4);
  
  printf("\n\nTimes sum was wrong: %d", sum_wrong);
  
  printf("\nthread_4 exiting");
  
  exit(0);
}
  


























