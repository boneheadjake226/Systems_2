/*
	Author:	Jake Hill
	Class:	CSE2431
	Lab: 	1
	
	TODO: Fill in compilation command
	Compilation command: $ gcc -o Lab1 hill_SysCall_Lab1.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(){
	int chars_read = 0, chars_wrote = 0;
	int i, file_xx, file_xyz, rw_return_val;
	char buffer[200];
	
	/*
		---Part 1
		***********************************************************************
	*/
	
	file_xx = open("xx.xx", 0);
	if(file_xx < 0){
		printf("Error opening file: /usr/class/cis/660/xx.xx \nError Value: %i", file_xx);
		return 1;
	}
	
	//TODO: Check file access rights for second param
	file_xyz = creat("XYZ.doc", 0751);
	if(file_xyz < 0){
		printf("Error creating file: XYZ.doc \nError Value: %i", file_xyz);
		return 1;
	}
	
	char abc_tag[4] = "ABC";
	do{
		rw_return_val = read(file_xx, buffer, 100);

		if(rw_return_val != 0){
			chars_read += rw_return_val;
			
			if(rw_return_val < 100){
				
				write(file_xyz, buffer, rw_return_val);

			}else{
				rw_return_val = write(file_xyz, buffer, 100);
			}
			
			write(file_xyz, abc_tag, 3);
			chars_wrote += rw_return_val + 3;
		}
	}while (rw_return_val != 0);

	close(file_xx);
	close(file_xyz);	
	printf("Characters read: %d \nCharacters wrote: %d \n", chars_read, chars_wrote);
	

	chars_read = 0;
	chars_wrote = 0;
	
	
	/*
		---Part 2
		*****************************************************************************
	*/
	
	int file_xxx, file_www, xxx_writes = 0, www_writes = 0;
	file_xyz = open("XYZ.doc", 0);
	if(file_xyz < 0){
		printf("Error opening file: /usr/class/cis/660/xx.xx \nError Value: %i", file_xx);
		return 1;
	}
	file_xxx = creat("XXX.txt", 0751);
	if(file_xxx < 0){
		printf("Error creating file: XXX.doc \nError Value: %i", file_xxx);
		return 1;
	}
	file_www = creat("WWW.txt", 0751);
	if(file_www < 0){
		printf("Error creating file: WWW.doc \nError Value: %i", file_www);
		return 1;
	}
	
	do{
		rw_return_val = read(file_xyz, buffer, 110);
		if(rw_return_val != 0){
			//Writing to XXX.txt
			
			/*
				-Filtering out '1's in most recent read
				-Replacing '1's with 'A'
			*/
			for(i = 0; i < rw_return_val; i++){
				if(buffer[i] == '1'){
					buffer[i] = 'A';
				}
			}
			
			xxx_writes += write(file_xxx, buffer, rw_return_val);
			
			rw_return_val = read(file_xyz, buffer, 150);
			if(rw_return_val != 0){
				//Writing to WWW.txt
				
				/*
					-Filtering out '2's in most recent read
					-Replacing '2's with "Ba"
				*/
				int j;
				for(i = 0; i < rw_return_val; i++){
					if(buffer[i] == '2'){
						for(j = rw_return_val - 1; j > i; j--){
							buffer[j+1] = buffer[j];
						}
						buffer[i] = 'B';
						buffer[i+1] = 'a';
						rw_return_val++;
					}
				}
				
				www_writes += write(file_www, buffer, rw_return_val);
			}
		}
	}while(rw_return_val != 0);
	
	
	printf("Chars written to XXX.txt: %i\nChars written to WWW.txt: %i\n", xxx_writes, www_writes);
	
	
	/*
		---Part 3
		*****************************************************************************
	*/
	
	//Seek to position 1000 of XXX.txt
	lseek(file_xxx, 1000, 0);
	
	//Write specified string at location overwriting existing chars
	strcpy(buffer, "12345678abcdefgh");
	write(file_xxx, buffer, 16);
	
	/*
		---Part 4
		*****************************************************************************
	*/
	

	
	//Seek to end of file WWW.txt
	lseek(file_www, 0, 2);
	
	strcpy(buffer, "Jake Hill");
	write(file_www, buffer, 9);
	
	/*
		---Part 5
		*****************************************************************************
	*/
	
	//Seek to position 900 of WWW.txt
	close(file_www);
	file_www = open("WWW.txt", 2);
	if(file_www < 0){
		printf("Error creating file: WWW.doc \nError Value: %i", file_www);
		return 1;
	}
	
	int file_www_end = lseek(file_www, 0, 2);
	int curr_offset = lseek(file_www, 900, 0);
	
	
	//make room to insert new string
	char write_buffer[26];
	char read_buffer[26]; 
	
	
	rw_return_val = read(file_www, write_buffer, 26);
	
	
	//printf("\nwrite_buffer : %s id: %d \nread_buffer: %s id: %d", write_buffer, &write_buffer, read_buffer, &read_buffer);

	
	
	printf("\nBefore loop rw_return_val: %d", rw_return_val);
	printf("\nfile end: %d \nCurrent Position: %d", file_www_end, curr_offset);
	while(rw_return_val != 0){
		rw_return_val = read(file_www, read_buffer, 26);
		lseek(file_www, -26, 1);
		write(file_www, write_buffer, rw_return_val);
		
		printf("\nWrite Buffer: %s", write_buffer);
		printf("\nrw_return val: %d", rw_return_val);
		
		for(i = 0; i < 26; i++){
			write_buffer[i] = read_buffer[i];
		}
		
	}
	
	lseek(file_www, 900, 0);
	strcpy(buffer, "abcdefghijklmnopQRSTUVWXYZ");
	write(file_www, buffer, 26);
	
	
	close(file_www);
	
	
	

	
	
	
	
	
	
	
	
	

	return 0;
}
