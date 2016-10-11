#include "ssem.h"
#include "sshm.h"
#include <stdio.h>

struct z_item{
	char * chars;
	int num;
} z_item;

int main(){
	int i, in = 0;
	int x_full, x_empty, y_full, y_empty, z_full, z_empty, w_sync;
	
	x_full = sem_open(60);
	x_empty = sem_open(61);
	y_full = sem_open(63);
	y_empty = sem_open(64);
	z_full = sem_open(65);
	z_empty = sem_open(66);
	w_sync = sem_open(67);
	
	//shared buffer
	struct z_item *buff_z;
	int buff_z_id = shm_get(101, (void**)&buff_z, 30*sizeof(z_item));
	
	sem_signal(w_sync);
	sem_signal(x_empty);
	sem_signal(y_empty);
	sem_wait(z_empty);
	sem_wait(z_empty);
	sem_wait(z_empty);
	
	printf("\nz made it to the for loop");
	for(i = 0; i < 500; i++){
		
		if( (i % 60) == 0 && i != 0){
			usleep(300);
		}
		 printf("\nz producing");
		struct z_item item;
		item.num = i + 1;
		item.chars = "ZZ";
		
		sem_wait(z_full);
		buff_z[in] = item;
		in = (in + 1) % 30;
		sem_signal(z_empty);
	}
	
	sem_signal(w_sync);
	
	return 0;
}