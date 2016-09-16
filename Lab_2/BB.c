#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

//for file opening and creating
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void main(){
	char *buffer[201];
	int file_xyz = creat("XYZ.txt",0777);
	
	printf("\nPid %d Code BB: Creating file XYZ.txt and writing 200 characters from file x.y", getpid());
	
	if(file_xyz < 0){
		printf("Error creating file XYZ.txt in BB.c");
		exit(0);
	}
	
	int file_xy = open("/usr/class/cis660/x.y", 0);
	if(file_xy < 0){
		prinf("Error opening file /usr/class/cis660/x.y in BB.c");
		exit(0);
	}
	
	//Read last 200 chars from x.y and write them to XYZ.txt
	seekl(file_xy, -200, 2);
	int rw_return = read(file_xy, buffer, 200);
	if(rw_return < 200){
		printf("Could not read 200 chars from x.y in BB.c");
		exit(0);
	}
	
	rw_return = write(file_xyz, buffer, 200);
	if(rw_return < 200){
		printf("Could not write all 200 chars to XYZ.txt in BB.c");
		exit(0);
	}
	
	if(close(file_xyz) < 0){
		printf("Closing error XYZ.txt in BB.c");
		exit(0);
	}
	if(close(file_xy) < 0){
		printf("Closing error x.y in BB.c");
		exit(0);
	}
	
	printf("\nPid %d Code BB: Done and terminating", getpid());
	
	exit(0);
}