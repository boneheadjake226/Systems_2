#include "ssem.h"
#include "sshm.h"
#include <stdio.h>

//Code of Proc3
int main(){
  int i, internal_reg;
  
  int s1 = sem_open(66);
  int s2 = sem_open(67);
  int s3 = sem_open(68);
  int s4 = sem_open(69);
  
  int *Account;
  shm_get(1256, (void**)&Account, 3*sizeof(int));
  
/* synchronize with Proc1, Proc3 and Proc4 (4 process 4 way synchronization)*/
  sem_signal(s1);
  sem_signal(s2);
  sem_signal(s4);
  sem_wait(s3);
  sem_wait(s3);
  sem_wait(s3);
  
  for (i = 0; i < 2000; i++){
    if( i == 500 || i == 1500){
      usleep(300);
    }
    
    if(i % 100 == 0){
      printf("\nFrom Process3 \n Account1: %d Account2: %d Account3: %d",
                Account[0], Account[1], Account[2]);
      printf("\nAccounts sum: %d", (Account[0] + Account[1] + Account[2]));
    }
    //Remove $200 from account 3 for transfer.
    sem_wait(s3);
    sem_wait(s1);
    internal_reg = Account [3];      /*Proc3 takes from Account[2]*/          
    internal_reg = internal_reg - 200;
    Account[3] = internal_reg;
    
    
    //Complete transfer from account 3 to account 1.
    
    internal_reg = Account [0];      /*Proc3 adds into Account[0]*/
    internal_reg = internal_reg + 200;
    Account[0] = internal_reg;
    sem_signal(s3);
    sem_signal(s1);
  }
  
  sem_signal(s4);
  
  return 0;
}