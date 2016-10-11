#include "ssem.h"
#include "sshm.h"
#include <stdio.h>

//Code of Proc2
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
  sem_signal(s3);
  sem_signal(s4);
  sem_wait(s2);
  sem_wait(s2);
  sem_wait(s2);
  
  for (i = 0; i < 2000; i++){
    
    if( (i%400) == 0 && i != 0){
      usleep(200);
    }

    if(i % 100 == 0){
      printf("\nFrom Process2 \n Account1: %d Account2: %d Account3: %d",
                Account[0], Account[1], Account[2]);
      printf("\nAccounts sum: %d", (Account[0] + Account[1] + Account[2]));
    }    
    //Remove $200 from account 2 for transfer.
    sem_wait(s1);
    sem_wait(s3);
    internal_reg = Account [1];      /*Proc3 takes from Account[2]*/          
    internal_reg = internal_reg - 200;
    Account[1] = internal_reg;
    
    
    //Complete transfer from account 2 to account 3
    internal_reg = Account [2];
    internal_reg = internal_reg + 200;
    Account[2] = internal_reg;
    sem_signal(s1);
    sem_signal(s3);
  }
  
  sem_signal(s4);
  
  return 0;
}


/*in the code above include some wait and signal operations on semaphores. Do not over-synchronize. */
