/*
	Author:	Jake Hill
	Class:	CSE2431
	Section: 12:45 W F
	Instructor: Gojko Babic
	Lab:	2
	
	Compilation Command: $make
						 $AA
*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

//for file opening and creating
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<signal.h>

void main(){
	int process_b, process_c1, process_c1a, process_d,
			process_c2, process_e, process_f;
	int return_code;
	
	//create Process B
	
	process_b = fork();
	if(process_b == 0){
		execv("BB",0);
	}
	
	//create Process C1
	process_c1 = fork();
	if(process_c1 == 0){
		execv("CC",0);
	}
	
	//create Process C1a
	process_c1a = fork();
	if(process_c1a == 0){
		execv("CC",0);
	}
	printf("\nPid %d Code AA: Creating process Pid %d (codeBB), process Pid %d (codeCC), and process Pid %d (codeCC)",
			getpid(), process_b, process_c1, process_c1a);
	
	//wait for child to terminate
	wait(&return_code);
	printf("\nPid $d Code AA: Process Pid %d terminated", getpid(), return_code);
	
	//create Process D
	process_d = fork();
	if(process_d == 0){
		execv("DD",0);
	}
	
	//create Process C2
	process_c2 = fork();
	if(process_c2 == 0){
		execv("CC",0);
	}
	
	//create Process E and wait for d to terminate
	process_e = fork();
	if(process_e == 0){
		execv("EE",0);
	}
	
	printf("\nPid %d Code AA: Created process Pid %d (Code DD), process Pid %d (Code CC), and process Pid %d (Code EE)",
				getpid(), process_d, process_c2, process_e);
	
	while(return_code != process_d){
		wait(&return_code);
		printf("\nPid %d Code AA: Process Pid %d terminated (not process D)", getpid(), return_code);
	}
	
	printf("\nPid %d Code AA: Process Pid %d (Code DD)terminated", getpid(), return_code);
	
	printf("\nPid %d Code AA: Killing process Pid %d (Code CC), process Pid %d (Code CC), and process Pid %d (Code CC)",
				getpid(), process_c1, process_c1a, process_c2);
	
	kill(process_c1, SIGKILL);
	printf("\nPid %d Code AA: Process Pid %d (Code CC) terminated", getpid(), process_c1);
	
	kill(process_c1a, SIGKILL);
	printf("\nPid %d Code AA: Process Pid %d (Code CC) terminated", getpid(), process_c1a);
	
	kill(process_c2, SIGKILL);
	
	
	printf("\nPid %d Code AA: Process Pid %d (Code CC) terminated, and waiting for 0.25", getpid(), process_c2);

	usleep(0.25);
	
	printf("\nPid %d Code AA: 0.25 sec elapsed.", getpid());
	
	int xyz_integer;
	char buffer[200];
	char int_buffer[200];
	
	printf("\nPid %d Code AA: Enter a character string: ", getpid());
	scanf(" %s", buffer);
	
	printf("\nPid %d Code AA: Enter a integer: ", getpid());
	scanf(" %d", &xyz_integer);
	
	sprintf(int_buffer, "%d", xyz_integer);
	
	char *args[] = {buffer, int_buffer, "Jake Hill", 0};
	
	process_f = fork();
	if(process_f == 0){
		execv("FF", args);
	}
	
	printf("\nPid %d Code AA: Created process Pid %d (Code FF) and terminating", getpid(), process_f);
	
	exit(0);
}