#include "ssem.h"
#include "sshm.h"
#include <stdio.h>

struct x_item{
	char * chars;
	int num;
} x_item;

/////////////////////////////////////////////////////////////
//Included to open and delete shared buffers, not modify.
struct y_item{
	//must always be "yyy"
	char * chars;
	int num;
} y_item;

struct z_item{
	//must always be "zz"
	char * chars;
	int num;
} z_item;
//////////////////////////////////////////////////////////////


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
	
	//shared buffers
	struct x_item *buff_x;
	int buff_x_id = shm_get(101, (void**)&buff_x, 20*sizeof(x_item));
	struct y_item *buff_y;
	int buff_y_id = shm_get(101, (void**)&buff_y, 30*sizeof(y_item));
	struct x_item *buff_z;
	int buff_z_id = shm_get(101, (void**)&buff_z, 25*sizeof(z_item));
	
	sem_signal(w_sync);
	sem_signal(y_empty);
	sem_signal(z_empty);
	sem_wait(x_empty);
	sem_wait(x_empty);
	sem_wait(x_empty);
	
	for(i = 0; i < 500; i++){
		
		if( (i % 100) == 0 && i != 0){
			usleep(300);
		}
		
		struct x_item item;
		item.num = i + 1;
		item.chars = "xx";
		
		sem_wait(x_full);
		buff_x[in] = item;
		in = (in + 1) % 20;
		sem_signal(x_empty);
	}
	
	/*
		sync with other processes before deleting shared mems
		and semaphores 
	*/
	sem_wait(w_sync);
	sem_wait(w_sync);
	sem_wait(w_sync);
	
	//Delete all semaphores and shared memories
	sem_rm(x_full);
	sem_rm(x_empty);
	sem_rm(y_full);
	sem_rm(y_empty);
	sem_rm(z_full);
	sem_rm(z_empty);
	sem_rm(w_sync);
	
	shm_rm(buff_x_id);
	shm_rm(buff_y_id);
	shm_rm(buff_z_id);
	
	return 0;
}
