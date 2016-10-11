#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

//for file opening and creating
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<signal.h>

void main(int arg_count, char *arg_pointer[]){
	
	printf("\nPid %d Code FF: argument 1: %s argument 2: %d argument 3: %s. Terminating", 
			getpid(), arg_pointer[0], arg_pointer[1], arg_pointer[2]);
	
	exit(0);
}