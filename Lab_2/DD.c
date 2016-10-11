#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

//for file opening and creating
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<signal.h>

void main(){
	int process_c3, process_ps;
	
	//create process C3 and wait 0.5
	process_c3 = fork();
	if(process_c3 == 0){
		execv("CC", 0);
	}
	printf("\nPid %d Code DD: Created process Pid %d (Code CC) and waiting for 0.5 seconds",
				getpid(), process_c3);
	
	usleep(0.5);
	
	process_ps = fork();
	if(process_ps == 0){
		execlp("/bin/ps", "ps", "-u","hillja", 0);
	}
	
	printf("\nPid %d Code DD: 0.5 sec elapsed. Created process Pid %d (ps), and waiting for it to terminate", 
				getpid(), process_ps);
	int return_code;
	wait(&return_code);
	printf("\nPid %d Code DD: Process Pid %d terminated", getpid(), return_code);
	
	kill(process_c3, SIGKILL);
	
	printf("\nPid %d Code DD: Killing process Pid %d terminated", getpid(), process_c3);
	
	printf("\nPid %d Code DD: Terminating", getpid());
	
	exit(0);
}