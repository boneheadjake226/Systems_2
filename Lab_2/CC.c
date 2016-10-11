#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

//for file opening and creating
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<signal.h>

void main(){
	
	printf("\nPid %d Code CC: Entering an infinite loop", getpid());
	while(1){
		usleep(100);
	}
	
	exit(0);
}