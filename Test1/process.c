/*************************************************
 * Author: ydlan
 * Date:   2018-10-16
 * Description:  Process and Thread

**************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<unistd.h>
#include<pthread.h>


//thread1 prime number
void *MyThread1(void){
	//sushu
	int i, j;
	int count;
	int n = 10;
	for(i = 2; i <= n; i++){
		count = 0;
		for(j = 2; j < i; j++){
		if( i % j == 0)
			count = 1;
		}

		if(count == 0)
			printf("%d\n", i);
	}
	printf("MyThread1 exit!\n");
    pthread_exit(0);
}


////thread2 fibonacci
void *MyThread2(void){
	//fibo
	int temp1=1, temp2=1, sum = 0;
	int n = 6;
	int i;
    for (i=1; i<=n; i++)
    {
    	if(i <= 2){
    		printf("1 ");
		}
		else{
			sum = temp1 + temp2;
        	temp1 = temp2;
        	temp2 = sum;
			printf("%d ", sum);	
		}
    }
    printf("\nMyThread2 exit!\n");
    pthread_exit(0);
}


//create two thread
void thread_two(void){

	pthread_t id1, id2;
	int ret1, ret2;
	ret1 = pthread_create(&id1, NULL, (void *)MyThread1, NULL);
	if(ret1 != 0){
		printf("thread1 create error!\n");
	}

	ret2 = pthread_create(&id2, NULL, (void *)MyThread2, NULL);
	if(ret2 != 0){
		printf("thread2 create error!\n");
	}

	pthread_join(id1,NULL); //wait thread1 finished
	pthread_join(id2,NULL); //wait thread2 finished
	printf("The no.5 process id is %d exit \n", getpid());

	exit(0);

}


void hello(void){
	execl("./hello", NULL);
	printf("The no.6 process id is %d exit \n", getpid());
	exit(0);
}


void fun(int i){
	switch(i){
		case 5: thread_two(); //create_thread_two
			break;
		case 6: hello();  //hello world;
			break;
	}
	exit(0);
}


//createprocess5, 6
int Createprocess_two(){
	int i;
	for(i = 5; i <= 6; i++){
		pid_t child2;
		child2 = fork();

		if(child2 == -1){
			printf("fork error!\n");
			return 0;
		}
		else if(child2 == 0){
			fun(i);
		}
	}

	for(i = 0; i < 2; i++){
		wait(NULL);
	}

	printf("The no.4 process id is %d exit \n", getpid());
	return 0;
	
}

//system calls
void sys(void){
	execlp("/bin/ls", "ls", NULL);

	printf("The no.3 process id is %d exit \n", getpid());
	exit(0);
}


void childfun(int i){
	switch(i){
		case 3: sys(); //sys calls
			break;
		case 4: Createprocess_two();
			break;
	}
}

//create process
int Createprocess(){
	int i;
	for(i = 3; i <= 4; i++){
		pid_t child;
		child = fork(); //create process

		if(child == -1){
			printf("fork error!\n");
			return 0;
		}
		else if(child == 0){
			childfun(i);
		}
	}

	for(i = 0; i < 2; i++){
		wait(NULL); //wait the process finished
	}

	printf("The no.1 process id is %d exit\n", getpid());
	return 0;
}


int main(){
	Createprocess();
	return 0;
}
