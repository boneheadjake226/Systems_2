/*
	Author:	Jake Hill
	Class:	CSE2431
	Lab: 	1
	
	TODO: Fill in compilation command
	Compilation command: $ gcc -o Lab1 hill_SysCall_Lab1.c -o sys_call
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>


int main(){
	int chars_read = 0, chars_wrote = 0;
	int i, file_xx, file_xyz, rw_return_val;
	char buffer[200];
	
	/*
		---Part 1
		***********************************************************************
	*/
	
	file_xx = open("/usr/class/cis660/xx.xx", 0);
	if(file_xx < 0){
		printf("Error opening file: /usr/class/cis660/xx.xx \nError Value: %i", file_xx);
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
	
	close(file_xxx);
	
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
	
	//shift untill end of file is read
	while(rw_return_val == 26){
		rw_return_val = read(file_www, read_buffer, 26);
		if(rw_return_val < 0){
			printf("\nRead Error from WWW.txt");
			exit(rw_return_val);
		}
		lseek(file_www, -26, 1);
		write(file_www, write_buffer, rw_return_val);
		
		for(i = 0; i < 26; i++){
			write_buffer[i] = read_buffer[i];
		}
		
	}
	//Shift original end of file
	rw_return_val = write(file_www, write_buffer, rw_return_val);
	if(rw_return_val < 0){
		printf("\nWrite Error to WWW.txt");
		exit(rw_return_val);
	}
	
	//Insert new string into allocated space
	lseek(file_www, 900, 0);
	strcpy(buffer, "abcdefghijklmnopQRSTUVWXYZ");
	write(file_www, buffer, 26);
	
	
	close(file_www);
	if(file_www < 0){
		printf("Error closing file WWW.txt");
		exit(file_www);
	}
	
	
	/*
		---Part 6
		*****************************************************************************
	*/	

	int file_zzz = creat("ZZZ.txt", 0751);
	if(file_zzz < 0){
		printf("\nError creating file ZZZ.txt");
		exit(file_zzz);
	}
	
	file_xxx = open("XXX.txt", 0);
	if(file_xxx < 0){
		printf("\nError opening file XXX.txt");
		exit(file_xxx);
	}
	
	//Copy XXX.txt into ZZZ.txt 200 chars at a time
	while(rw_return_val != 0){
		rw_return_val = read(file_xxx, buffer, 200);
		if(rw_return_val < 0){
			printf("Read Error XXX.txt");
			exit(rw_return_val);
		}
		
		rw_return_val = write(file_zzz, buffer, rw_return_val);
		if(rw_return_val < 0){
			printf("Write Error ZZZ.txt");
			exit(rw_return_val);
		}
	}
	
	if(close(file_xxx) < 0){
		printf("Close error XXX.txt");
		exit(1);
	}
	
	if(close(file_zzz) < 0){
		printf("Close error XXX.txt");
		exit(1);
	}
	
	
	/*
		---Part 7
		*****************************************************************************
	*/
	
	chars_wrote = 0;
	file_www = open("WWW.txt", 0);
	if(file_www < 0){
		printf("Open error WWW.txt");
		exit(file_www);
	}
	file_zzz = open("ZZZ.txt", 1);
	if(file_zzz < 0){
		printf("Open error ZZZ.txt");
		exit(file_zzz);
	}
	
	lseek(file_www, -1, 2);
	lseek(file_zzz, 0, 2);
	
	//Write WWW.txt in reverse one char at a time
	while(lseek(file_www, 0, 1) != 0){
		read(file_www, buffer, 1);
		chars_wrote += write(file_zzz, buffer, 1);
		lseek(file_www, -2, 1);
	}
	//Write first character in WWW.txt to end of ZZZ.txt
	read(file_www, buffer, 1);
	chars_wrote += write(file_zzz, buffer, 1);
	
	if(close(file_zzz) < 0){
		printf("Close error XXX.txt");
		exit(1);
	}
	
	if(close(file_www) < 0){
		printf("Error closing file WWW.txt");
		exit(file_www);
	}
	
	printf("Chars wrote to ZZZ.txt: %d\n", chars_wrote);
	
	/*
		---Part 8
		*****************************************************************************
	*/		
	
	file_zzz = open("ZZZ.txt", 0);
	i = 1;
	char sub_buff[81];
	
	while(read(file_zzz, buffer, 80) > 0){
		if(i == 100){
			memcpy(sub_buff, buffer, 80);
			printf("%s \n", sub_buff);
			i = 1;
		}else{
			i++;
		}
	}
	
	if(close(file_zzz) < 0){
		printf("Close error XXX.txt");
		exit(1);
	}
	
	if(unlink("XXX.txt") < 0){
		printf("Error Deleting file XXX.txt\n");
		exit(1);
	}
	if(unlink("ZZZ.txt") < 0){
		printf("Error Deleting file ZZZ.txt\n");
		exit(1);
	}
	if(unlink("WWW.txt") < 0){
		printf("Error Deleting file WWW.txt\n");
		exit(1);
	}	
	
	

	return 0;
}
