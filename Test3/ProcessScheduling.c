/*******************************************************************
 * Author: ydlan
 * Date:   2018-11-14
 * Description:   Process Scheduling 

 *******************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>
#include<pthread.h>
#include<time.h>

#define THREADNUM 20  //the child thread number
pthread_mutex_t DeviceMutex;

//virtual PCB
struct VirtualPCB
{
	int tid; //thread id
	int priority; //thread priority
	int arrivetime;
	int waittime;
	int runtime;
	int visited;
	int tempruntime; 
	
}PCB[THREADNUM];

//initPCB
void initPCB(){
	int n;
	srand(time(NULL));
	
	for(n = 0; n < THREADNUM; n++){
		PCB[n].tid = n + 1;
		PCB[n].priority = 1 + rand()%19;
		PCB[n].runtime = 1 + rand()%19;

		PCB[n].tempruntime = PCB[n].runtime;
		PCB[n].arrivetime = 0;
		PCB[n].waittime = 0;
		PCB[n].visited = 0;		
	}
	
}

//FCFS,first come first sever 
void useFCFS(){
	printf("\nUsing FCFS...\n");
	int i, j;
	int start = 0;
	float waittime = 0;
	float avwait = 0; //average wait time
	
	for(i = 0; i < THREADNUM/2; i++){
		for(j = 0; j < THREADNUM; j++){
			if(PCB[j].arrivetime == i && PCB[j].visited == 0){
				printf("Thread:%2d Start:%2d Runtime:%2d\n", PCB[j].tid, start, PCB[j].runtime);
				waittime = waittime + (float)start;
				start = start + PCB[j].runtime;
				PCB[j].visited = 1;
			}
		}
	} 
	//calculate the average waitting time
	avwait = waittime / (float)THREADNUM;
	printf("Total waitting time:%f\n", waittime);
	printf("Average waitting time:%f\n", avwait);
}



//SJF shortest job first
void useSJF(){
	//int the thread the visited
	for(int k = 0; k < THREADNUM; k++){
		PCB[k].visited = 0;
	}

	printf("\nUsing SJF...\n");
	int i, j;
	int start = 0;
	float waittime = 0;
	float avwait = 0; //average wait time
	
	for(i = 0; i < THREADNUM; i++){
		for(j = 0; j < THREADNUM; j++){
			if(PCB[j].runtime == i && PCB[j].visited == 0){
				printf("Thread:%2d Start:%2d Runtime:%2d\n", PCB[j].tid, start, PCB[j].runtime);
				waittime = waittime + (float)start;
				start = start + PCB[j].runtime;
				PCB[j].visited = 1;
			}
		}
	} 
	//calculate the average waitting time
	avwait = waittime / (float)THREADNUM;
	printf("Total waitting time:%f\n", waittime);
	printf("Average waitting time:%f\n", avwait);
}



//Priority
void usePriority(){
	//int the thread the visited
	for(int k = 0; k < THREADNUM; k++){
		PCB[k].visited = 0;
	}

	printf("\nUsing Priority...\n");
	int i, j;
	int start = 0;
	float waittime = 0;
	float avwait = 0; //average wait time
	
	for(i = 0; i < THREADNUM; i++){
		for(j = 0; j < THREADNUM; j++){
			if(PCB[j].priority == i && PCB[j].visited == 0){
				printf("Thread:%2d Start:%2d Runtime:%2d  Priority:%2d\n", PCB[j].tid, start, PCB[j].runtime, PCB[j].priority);
				waittime = waittime + (float)start;
				start = start + PCB[j].runtime;
				PCB[j].visited = 1;
			}
		}
	} 
	//calculate the average waitting time
	avwait = waittime / (float)THREADNUM;
	printf("Total waitting time:%f\n", waittime);
	printf("Average waitting time:%f\n", avwait);
}


//RR
void useRR(int r){

	printf("\nUsing RR...\n");
	
	int start = 0;
	float waittime = 0;
	float avwait = 0; //average wait time

	for(int k = 0; k < THREADNUM; k++){
		PCB[k].visited = 0;
	}

	//r is timeblock
	for(int i = 0; i < 20*THREADNUM; i = i + r){
		int j = (i % (20 * r))/r;

		if(PCB[j].tempruntime > 0){
			int tempruntime = r;

			if(PCB[j].tempruntime - r <= 0){
				tempruntime = PCB[j].tempruntime;
				PCB[j].waittime = start + tempruntime - PCB[j].runtime;
				//PCB[j].visited = 1;

			}
			printf("Thread:%2d Start:%2d Runtime:%2d\n", PCB[j].tid, start, tempruntime);
			start = start + tempruntime;
			PCB[j].tempruntime = r;
		}
	}


	for(int m = 0; m < THREADNUM; m++){
		waittime = waittime + PCB[m].waittime;
	}

	avwait = waittime / (float)THREADNUM;
	printf("Total waitting time:%f\n", waittime);
	printf("Average waitting time:%f\n", avwait);
}


//print the thread's func
void *t_print(void *k){

	int n = *(int *)k;
	
	while(1){
		pthread_mutex_lock(&DeviceMutex);
		printf("thread%-3d", n);
		printf("tid:%2d Priority:%2d runtime:%2d\n", PCB[n-1].tid, PCB[n-1].priority, PCB[n-1].runtime);
		pthread_mutex_unlock(&DeviceMutex);
		sleep(1);
		break;
	} 
	
	pthread_exit(0); //the thread exit
	
}


void *Children(){

	initPCB();
	int ret[THREADNUM];
	pthread_t tid[THREADNUM]; //create child pthread
	pthread_mutex_init(&DeviceMutex, NULL);

	int i, j;
	for(i = 0; i < THREADNUM; i++){
		int k = i + 1;
		ret[i] = pthread_create(&tid[i], NULL, &t_print, &k);//create the thread

		if(ret[i] == 0){
			sleep(1);
		}
		else{
			printf("thread_%2d failed\n", i+1);
		}

	}

	for(j = 0; j < THREADNUM; j++){
		pthread_join(tid[i], NULL); //wait the child thread finish
	}

	pthread_mutex_destroy(&DeviceMutex);
	pthread_exit(0);
}



int main(){
	int ret1;
	pthread_t id1; //create father pthread
	ret1 = pthread_create(&id1, NULL, &Children, NULL);

	if(ret1 == 0){
		printf("create child thread...\n");
		sleep(20);
	}
	else{
		printf("create main thread failed\n");
	}

	useFCFS();
	useSJF();
	usePriority();

	int r; //the time slice
	printf("Input RR time...\n");
	scanf("%d", &r);
	useRR(r);

	return 0;
}




