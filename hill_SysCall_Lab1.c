/*
	Author:	Jake Hill
	Class:	CSE2431
	Lab: 	1
	
	TODO: Fill in compilation command
	Compilation command: $ gcc 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(){
	FILE *file_xyz = fopen("XYZ.doc", "w");
	FILE *file_xx = fopen("/usr/class/cis660/xx.xx", "r");
	int chars_read = 0, chars_wrote = 0;
	int not_eof_check = 1;
	int i;
	
	char next_char;
	
	//Check if files are open
	if(file_xyz == NULL){
		printf("Open/creation of file XYZ.doc has failed\n");
		return 0;
	}
	if(file_xx == NULL){
		printf("Open/creation of file /usr/class/cis660/xx.xx has failed\n");
		return 0;
	}
	
	do{
		while(fscanf(file_xx, "%c", next_char) != eof && i < 100){
			fprintf(file_xyz, "%c", next_char);
			
			chars_read++;
			chars_wrote++;
			i++;
		}
		
		if(i < 100){
			not_eof_check = 0;
		}else{
			i = 0;
		}
		
		fprintf(file_xyz, "ABC");
	}while(not_eof_check);
	
	
	
	
	
	return 0;
}
