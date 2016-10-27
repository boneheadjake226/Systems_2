/*-------------------------------------|
  Author: Jake Hill
  Class: CSE 2431
  Instructor: Gojko Babic
  Lab: 4
  Part: B
---------------------------------------|*/

#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NUM_THREADS 3
#define BUFF_X_SIZE 20
#define BUFF_Y_SIZE 40

typedef struct {
  int num;
  char chars[6];
} x_item;

typedef struct {
  int num;
  char chars[4];
} y_item;

/*--- Producer Buffers ---*/
x_item buff_x[BUFF_X_SIZE];
y_item buff_y[BUFF_Y_SIZE];

/*--- Semaphores ---*/
sem_t x_empty, y_empty, buffs_full, a_sync, b_sync, mutex;

/*--- Thread Prototype Functions ---*/
void *thread_a();
void *thread_b();
void *thread_c();

pthread_t tid[NUM_THREADS];

int items_in_x = 0;

int main(){
  int i;
  
  /*--- Create all semaphores and threads ---*/
  if(sem_init(&x_empty, 0, BUFF_X_SIZE) < 0){
    printf("\nFailed to open semaphore: x_empty");
    return -1;
  }
  if(sem_init(&y_empty, 0, BUFF_Y_SIZE) < 0){
    printf("\nFailed to open semaphore: y_empty");
    return -1;
  }
  if(sem_init(&buffs_full, 0, 0) < 0){
    printf("\nFailed to open semaphore: buffs_full");
    return -1;
  }
  if(sem_init(&a_sync, 0, 0) < 0){
    printf("\nFailed to open semaphore: a_sync");
    return -1;
  }
  if(sem_init(&b_sync, 0, 0) < 0){
    printf("\nFailed to open semaphore: b_sync");
    return -1;
  }
  if(sem_init(&mutex, 0, 1) < 0){
    printf("\nFailed to open semaphore: ,mutex");
    return -1;
  }
  
  if(pthread_create(&tid[0], NULL, thread_a, NULL) < 0){
    printf("\nFailed to create thread_a");
    return -1;
  }
  if(pthread_create(&tid[1], NULL, thread_b, NULL) < 0){
    printf("\nFailed to create thread_b");
    return -1;
  }
  if(pthread_create(&tid[2], NULL, thread_c, NULL) < 0){
    printf("\nFailed to create thread_c");
    return -1;
  }
  
  for(i = 0; i < NUM_THREADS; i++){
    pthread_join(tid[i], NULL);
  }
  return 0;
}

/*--- Producer A ---*/
void *thread_a(){
  int i, x = 0;
  const int ITERATIONS = 3000;
  
  /*--- Sync Point Thread A ---*/
  if(sem_post(&b_sync) < 0){ 
    printf("\n|thread_a| Error sem_post @ sync point");
    pthread_exit(NULL);
  }
  if( sem_wait(&a_sync) < 0 ){
    printf("\n|thread_a| Error sem_wait @ sync point");
    pthread_exit(NULL);
  }
  
  for(i = 0; i < ITERATIONS; i++){
    if((i % 500) == 0 && i != 0){
      usleep(100000);
    }
    
    /*--- Produce Item ---*/
    x_item item;
    item.num = i + 1;
    if( i%2 == 0){
      strcpy(item.chars, "ABCaa");
      item.chars[5] = 0;
    }else{
      strcpy(item.chars, "XYZxx");
      item.chars[5] = 0;
    }
    
    /*--- Place Item In Buffer X ---*/
    if(sem_wait(&x_empty) < 0){ printf("\n|thread a| Error sem_wait(&x_empty)"); pthread_exit(NULL);}
    buff_x[x] = item;
    x = (x + 1) % BUFF_X_SIZE;
    
    if(sem_wait(&mutex) < 0){ printf("\n|thread a| Error sem_wait(&mutex)"); pthread_exit(NULL);}
    items_in_x++;
    if(sem_post(&mutex) < 0){ printf("\n|thread a| Error sem_post(&mutex)"); pthread_exit(NULL);}
    
    if(sem_post(&buffs_full) < 0){ printf("\n|thread a| Error sem_post(&buffs_full)"); pthread_exit(NULL);}
    
  }
  
}

/*--- Producer B ---*/
void *thread_b(){
  int i, y = 0;
  const int ITERATIONS = 4000;
  
  /*--- Sync Point Thread A ---*/
  if(sem_post(&a_sync) < 0){ 
    printf("\n|thread_b| Error sem_post @ sync point");
    pthread_exit(NULL);
  }
  if( sem_wait(&b_sync) < 0 ){
    printf("\n|thread_b| Error sem_wait @ sync point");
    pthread_exit(NULL);
  }
  
  for(i = 0; i < ITERATIONS; i++){
    if((i % 1000) == 0 && i != 0){
      usleep(200000);
    }
    
    /*--- Produce Item ---*/
    y_item item;
    item.num = i + 1;
    if( i > 2000){
      strcpy(item.chars, "MMM");
      item.chars[3] = 0;
    }else{
      strcpy(item.chars, "NNN");
      item.chars[3] = 0;
    }
    
    /*--- Place Item In Buffer Y ---*/
    if(sem_wait(&y_empty) < 0){ printf("\n|thread b| Error sem_wait(&y_empty)"); pthread_exit(NULL);}
    buff_y[y] = item;
    y = (y + 1) % BUFF_Y_SIZE;
    if(sem_post(&buffs_full) < 0){ printf("\n|thread b| Error sem_post(&buffs_full)"); pthread_exit(NULL);}
    
  }
}


/*--- Consumer ---*/
void *thread_c(){
  int i, x = 0, y = 0;
  const int ITERATIONS = 7000;
  void * item;
  
  for(i = 0; i < ITERATIONS; i++){
    if((i % 2000) == 0 && i != 0){
      usleep(300000);
    }
    
    
    /*--- Check if either buffer has items to consume ---*/
    if(sem_wait(&buffs_full) < 0){ printf("\n|thread c| Error sem_wait(&buffs_full)"); pthread_exit(NULL);}
    
    /*--- Can I take from buff_x? ---*/
    if(items_in_x > 0){
      item = &buff_x[x];
      x = (x + 1) % BUFF_X_SIZE;
      
      if(sem_wait(&mutex) < 0){ printf("\n|thread c| Error sem_wait(&mutex)"); pthread_exit(NULL);}
      items_in_x--;
      if(sem_post(&mutex) < 0){ printf("\n|thread c| Error sem_post(&mutex)"); pthread_exit(NULL);}
      
      printf("\n|thread c| Item %d: %d%s", (i + 1), ((x_item*)item)->num, ((x_item*)item)->chars);
      if(sem_post(&x_empty) < 0){ printf("\n|thread c| Error sem_post(%x_empty)"); pthread_exit(NULL);}
    } else{
      item = &buff_y[y];
      y = (y + 1) % BUFF_Y_SIZE;
      
      printf("\n|thread c| Item %d: %d%s", (i + 1), ((y_item*)item)->num, ((y_item*)item)->chars);
      
      if(sem_post(&y_empty) < 0){ printf("\n|thread c| Error sem_post(%y_empty)"); pthread_exit(NULL);}
    }
  }
}

















