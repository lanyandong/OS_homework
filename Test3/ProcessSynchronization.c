/*******************************************************************
 * Author: ydlan
 * Date:   2018-11-14
 * Description:   Process Synchronization (Philosopher's Dining Solution)

 *******************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>
#include<pthread.h>
#include<semaphore.h>

#define N 5
#define LEFT (i+N-1)%N  //the left of Philosopher
#define RIGHT (i+1)%N  //the right of Philosopher

#define THINK_TIME 2
#define EAT_TIME 2
sem_t chopstick[N]; //semaphore


//state of thinking
void think(int i)
{
    printf("Philosopher %d is thinking...\n", i);
    sleep(THINK_TIME);
}

//state of eating
void eat(int i)
{
    printf("Philosopher %d is eating...\n", i);
    sleep(EAT_TIME);
}


void *phi(void *arg)
{
    int i = *(int *)arg;
    while (1)
    {
    	if( i % 2 == 0){
    		think(i);
			sem_wait(&chopstick[LEFT]);  //take left chopstick
			sem_wait(&chopstick[RIGHT]); //take right chopstick
       		eat(i);
        	sem_post(&chopstick[LEFT]);  //put left chopstick
			sem_post(&chopstick[RIGHT]); //put right chopstick		
		}
		else{
			think(i);
			sem_wait(&chopstick[RIGHT]);  //take left chopstick
			sem_wait(&chopstick[LEFT]);   //take right chopstick
       		eat(i);
       		sem_post(&chopstick[RIGHT]);//put right chopstick
       		sem_post(&chopstick[LEFT]);//put left chopstick	
       			
		}
      
    }
    
    return NULL;
}


int main()
{
	
	//sem_init(&mutex,0,1);
	for(int i=0; i<N; i++){
		sem_init(&chopstick[i],0,1);
	}
	
    int ret[N];
    pthread_t tid[N];  //create the pthread
    for (int i=0; i<N; i++){

		ret[i]=pthread_create(&tid[i], NULL, &phi, &i);
        
        if(ret[i] == 0){
        	printf("Thread %d create succeed.\n", i);
        	sleep(2);
        }
        else{
        	printf("thread_%2d failed\n", i);
        }
    }

    for (int i = 0; i < N; i++){
        pthread_join(tid[i], NULL); //wait the thread finished
    }

    //sem_destroy(&chopstick);
    for(int i=0;i<N;i++){
		sem_destroy(&chopstick[i]);
	}


    return 0;
}

