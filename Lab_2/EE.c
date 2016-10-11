#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

//for file opening and creating
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<signal.h>

void main(){
	int file_xyz = open("XYZ.txt", 0), rw_return;
	char *buffer[2];
	if(file_xyz < 0){
		printf("Error opening XYZ.txt in EE.c");
		exit(0);
	}
	
	printf("\nPid %d Code EE: This is what I have read from XYZ.txt: ");
	
	while(read(file_xyz, buffer, 1) != 0){
		printf("%c", buffer[0]);
	}
	
	close(file_xyz);
	unlink("XYZ.txt");
	
	printf("\nPid %d Code EE: Deleted XYZ.txt and I am terminating", getpid());
	
	
	
	exit(0);
}