#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct {
  int num;
  char chars[6];
} x_item;

typedef struct {
  int num;
  char chars[4];
} y_item;


x_item buff_x[20];
y_item buff_y[40];

sem_t x_empty, x_full, y_empty, y_full;

void *thread_a();
void *thread_b();
void *thread_c();

pthread_t tid[3];

int main(){
  
};

void *thread_a(){
  
  
}

void *thread_b(){
  
  
}

void *thread_c(){
  
  
}