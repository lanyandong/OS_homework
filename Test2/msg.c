/*******************************************************************
 * Author: ydlan
 * Date:   2018-11-1
 * Description:  Process communication(message queue)

 *******************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<unistd.h>
#include<string.h>
#include<sys/msg.h>
#include<sys/ipc.h>

// the msg struct
typedef struct mymsg{
	long mytype; //msg type
	unsigned char mytext[128];
		
}mymsg;


//open file and write
void WriteFile(){
 	FILE *file;
 	if((file = fopen("test2.txt", "w")) == NULL ){
 		printf("file open fail！\n");
 		exit(1);
 	}
 	else{
 		fputs("helloabc111", file);
 	}
 	fclose(file);
}

//read the msg, then read and print the text;
void ReadFile(mymsg msg, char *r_msg){
	if((strcmp(msg.mytext, r_msg)) == 0){
    //accept the msg succeful;
        FILE *file; //readfile
        if((file = fopen("test2.txt","r")) == NULL){
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


void childfun(int i,  int msqid){
	switch(i){
		case 1:{
			WriteFile();
            //string w_msg = "ok";
            mymsg msg = {0};
            msg.mytype = 1;
            strcpy(msg.mytext, "ok");
            //write msg to msgqueue
            msgsnd(msqid, &msg, sizeof(msg.mytext), IPC_NOWAIT); //msgsend() send the msg
            break;
		} 


		case 2: {
        	mymsg msg = {0};
            //read msg from msgqueue
            msgrcv(msqid, &msg, sizeof(msg.mytext), 0, IPC_NOWAIT);//msgrcv() receive the msg
            
			char r_msg[] = "ok";
            ReadFile(msg, r_msg);
            break;
		} 		
	}
	exit(0);
}


//get the queue id
int getqid(){
	key_t key = ftok(".", 't');
	int msqid = msgget(key, 0666 | IPC_CREAT);
	return msqid;
}


int main(){
	////create msg queue
	int msqid = getqid();
	
	///create process 
	int i;
	for(i = 1; i <= 2; i++){
		pid_t child;
		child = fork();//create the process

		if(child == -1){
			printf("fork error!\n");
			return 0;
		}
		else if(child == 0){
			childfun(i, msqid);
		}
	}
	for(i = 0; i < 2; i++){
		wait(NULL); //wait the process finished
	}
	
	printf("The no.1 process id is %d exit\n", getpid());
	return 0;
}
