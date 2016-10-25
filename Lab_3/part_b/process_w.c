#include "ssem.h"
#include "sshm.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>

typedef struct{
	//must always be "xx"
	char chars[3];
	int num;
} x_item;

typedef struct{
	//must always be "yyy"
	char chars[4];
	int num;
} y_item;

typedef struct{
	//must always be "zz"
	char chars[3];
	int num;
} z_item;

int main(){
	int x = 0, y = 0, z = 0, i = 0;
	int x_full, x_empty, y_full, y_empty, z_full, z_empty, w_sync;
	
	//shared buffers
	x_item *buff_x;
	shm_get(101, (void**)&buff_x, 20*sizeof(x_item));
	y_item *buff_y;
	shm_get(102, (void**)&buff_y, 30*sizeof(y_item));
	z_item *buff_z;
	shm_get(103, (void**)&buff_z, 25*sizeof(z_item));
	
	//item slots for consumption
	//struct z_item item_z;
	//struct y_item item_y;
	//struct x_item item_x;
	
	//buffer access semaphores
	x_full = sem_create(60, 20);
	x_empty = sem_create(61, 0);
	y_full = sem_create(63, 30);
	y_empty = sem_create(64, 0);
	z_full = sem_create(65, 25);
	z_empty = sem_create(66, 0);
	w_sync = sem_create(67, 0);
	
	
	for(i = 0; i < 500; i++){
		
		if( (i % 100) == 0 && i != 0){
			usleep(300);
		}
		sem_wait(x_empty);
		x_item item_x = buff_x[x];
		x = (x + 1) % 20;
		sem_signal(x_full);
		
		
		
		sem_wait(y_empty);
		y_item item_y = buff_y[y];
		y = (y + 1) % 30;
		sem_signal(y_full);

		
		sem_wait(z_empty);
		z_item item_z = buff_z[z];
		z = (z + 1) % 25;
		sem_signal(z_full);
		
		printf("\n %d%s %s%d %d%s", item_x.num, item_x.chars, item_y.chars, item_y.num, item_z.num, item_z.chars);
	}
	
	sem_signal(w_sync);
	
	return 0;
}


























