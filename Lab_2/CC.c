#include <unistd.h>
#include <stdio.h>

//for file opening and creating
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void main(){
	
	printf("\nPid %d Code CC: Entering an infinite loop", getpid());
	while(true){
		usleep(100);
	}
	
	exit(0);
}