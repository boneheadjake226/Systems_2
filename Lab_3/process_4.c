#include "ssem.h"
#include "sshm.h"
#include <stdio.h>

int main(){
  int i, account_sum, sum_wrong = 0;
  
  int s1 = sem_open(66);
  int s2 = sem_open(67);
  int s3 = sem_open(68);
  int s4 = sem_open(69);
  
  int *Account;
  int accounts_id = shm_get(1256, (void**)&Account, 3*sizeof(int));
  
  //4-process 4-way sync point
  sem_signal(s1);
  sem_signal(s2);
  sem_signal(s3);
  sem_wait(s4);
  sem_wait(s4);
  sem_wait(s4);
  
  for( i = 0; i < 5000; i++){
    if( (i%1000) == 0 && i != 0){
      usleep(50);
    }
    
    account_sum = Account[0] + Account[1] + Account[2];
    
    if(account_sum != 30000){
      sum_wrong++;
    }
  }
  
  //4 process 1 way sync
  //Makes sure process4 does not terminate prematurely
  wait(s4);
  wait(s4);
  wait(s4);
  
  printf("\nTimes account sum was not $30,000: %d", sum_wrong);
  
  sem_rm(s1);
  sem_rm(s2);
  sem_rm(s3);
  sem_rm(s4);
  
  shm_rm(accounts_id);
  
  return 0;
}