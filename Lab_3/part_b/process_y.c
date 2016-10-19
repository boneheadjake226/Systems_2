#include "ssem.h"
#include "sshm.h"
#include <stdio.h>
#include <unistd.h>

#define YYY "YYY"
typedef struct y_item{
	char * chars;
	int num;
};

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
	y_item *buff_y;
	int buff_y_id = shm_get(102, (void**)&buff_y, 30*sizeof(y_item));
	
	sem_signal(w_sync);
	sem_signal(x_empty);
	sem_signal(z_empty);
	sem_wait(y_empty);
	sem_wait(y_empty);
	sem_wait(y_empty);
	

	for(i = 0; i < 500; i++){
		if( (i % 75) == 0 && i != 0){
			usleep(300);
		}
		y_item item = {.num = i + 1, .chars = "YYY"};
		//item.num = i + 1;
		//char yyy[4] = "YYY";
		//item.chars = yyy;
		//printf(" %d%s", item.num, item.chars);
		
		sem_wait(y_full);
		buff_y[in] = item;
		in = (in + 1) % 30;
		sem_signal(y_empty);
	}
	
	sem_signal(w_sync);
	
	return 0;
}
