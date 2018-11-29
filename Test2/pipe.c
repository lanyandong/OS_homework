/*******************************************************************
 * Author: ydlan
 * Date:   2018-11-1
 * Description:  Process communication(pipe)

 *******************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<unistd.h>
#include<string.h>

//open file and write
void WriteFile(){
 	FILE *file;
 	if((file = fopen("test.txt", "w")) == NULL ){
 		printf("file open fail！\n");
 		exit(1);
 	}
 	else{
 		fputs("abc123", file); //write the file
 	}
 	fclose(file); //close file
}

//read the msg, then read and print the text;
void ReadFile(char *readbuffer, char *r_msg){
	if((strcmp(readbuffer, r_msg)) == 0){
    //accept the msg succeful;
        FILE *file; //readfile
        if((file = fopen("test.txt","r")) == NULL){
            printf("the file open fail!\n");
            exit(1);
        }
        else{
	        while(!feof(file)){
	            char text[200];
	            if(fgets(text, 200, file) != NULL){
	                printf("%s", text);
	                printf("\n");
	            }
        	}

        fclose(file); //close file
        }

	}
	else{
		printf("the msg is diffence!\n");
	}

}


void childfun(int i, int w_fd, int r_fd){
	switch(i){
		case 1:{
			WriteFile();
            char w_msg[] = "ok";
            write(w_fd, w_msg, sizeof(w_msg)); //write the msg
            close(w_fd);
            break;
		} 
		
		case 2: {
            char readBuffer[200];
            read(r_fd, readBuffer, sizeof(readBuffer)); //read the msg
            char r_msg[] = "ok";
            ReadFile(readBuffer, r_msg);
            break;
		} 		
	}
	exit(0);
}


int main(){
	int fd[2];
	pipe(fd);//create the pipe

	int w_fd = fd[1]; //the write port
	int r_fd = fd[0]; //the read port

	int i;
	for(i = 1; i <= 2; i++){
		pid_t child;
		child = fork(); //crate the process

		if(child == -1){
			printf("fork error!\n");
			return 0;
		}
		else if(child == 0){
			childfun(i, w_fd, r_fd);
		}
	}

	for(i = 0; i < 2; i++){
		wait(NULL);	//wait the process finished
	}

	printf("The no.1 process id is %d exit\n", getpid());

	return 0;
}
