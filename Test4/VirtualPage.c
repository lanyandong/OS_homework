/*************************************************
 * Author: ydlan
 * Date:   2018-11-25
 * Description:  Memory management the Virtual paging

**************************************************/



#include<stdio.h>
#include<stdlib.h>

#define MAX 5000
#define TRUE 1
#define FALSE 0

int offset = 0;

int u[MAX];    //check is 1 or 0 
int time[MAX]; //the visited time
int timeflag; //time flag


//first in first out
void fifo(int frame[], int input, int frameSize){
	offset = offset % frameSize;
	frame[offset] = input; //no vpage id and replace
	offset = offset + 1;

}
//find shot or miss
int fifofind(int page[], int a, int size){
	int flag;
	flag = FALSE;
	for(int i = 0; i < size; i++){
		if(page[i] == a){
			flag = TRUE;
		}
	}
	return flag;
}




// clock two time chance 
void clock(int frame[], int input, int frameSize){
	
	while(1){
		offset = offset % frameSize;
		if(u[offset] == 1){
			u[offset] = 0;
		}
		else if(u[offset] == 0){
			frame[offset] = input; //no vpage id and replace
			u[offset] = 1;
			break;
		}
		offset++;
	}	
}

//find shot or miss
int clockfind(int page[], int a, int size){
	int flag;
	flag = FALSE;
	for(int i = 0; i < size; i++){
		if(page[i] == a){
			flag = TRUE;
			u[i] = 1; // make 1
		}
	}
	return flag;
}



//complete LRU algorithm
void lru(int frame[], int input, int frameSize){
	int offset = 0;
	for(int i = 0; i < frameSize; i++){
		if(time[i] < time[offset]){
			offset = i;
		}
	}

	frame[offset] = input; //no vpage id and replace
	time[offset] = timeflag;	
}

//find shot or miss
int lrufind(int page[], int a, int size){
	int flag;
	flag = FALSE;
	for(int i = 0; i < size; i++){
		if(page[i] == a){
			flag = TRUE;
			time[i] = timeflag; //visted time
		}
	}
	return flag;
}




int main(int argc, char const *argv[]){
	
	int page[MAX]; //the page
	int size[] = {100, 500, 1000, 2000, 5000}; //the vpage size
	int miss = 0; //the miss pages

	FILE *fp[5]; //the file pointer order to read workloads
	int j;

	printf("\n***************fifo***************");
	// total 5 page size 100, 500, 1000, 2000, 5000
	for(j = 0; j < 5; j++){

		printf("\nthe page size is %d\n", size[j]);
		fp[0] = fopen("workload7", "r");
		fp[1] = fopen("workload8", "r");
		fp[2] = fopen("workload9", "r");
		fp[3] = fopen("workload10", "r");
		fp[4] = fopen("workload11", "r");
		fp[5] = fopen("workload12", "r");

		int c = 0; //save the vpage id 
		int count; //record the vpage number
		int i;

		// total read 6 workdloads
		for(i = 0; i < 6; i++){
			count = 0;
			miss = 0;
			offset = 0;

			//read the workloads data until end of file
			while(fscanf(fp[i], "%d", &c) != EOF){

					if(fifofind(page, c, size[j]) == FALSE){
						fifo(page, c, size[j]);
						miss++;
					}

				count++;

				//order to qiuck,just read the 10000 vpages
				if(count >= 10000){
					break;
				}
			}

			//calculate the miss page rate 
			double missrate = miss * 1.0 / count * 100;
			printf("workload%2d  Count is:%d  missNum is:%6d  missRate is:%f%%\n", i + 7, count, miss, missrate);
			fclose(fp[i]);  //close the file pointer
		}

	}


	printf("\n***************clock***************");
	// total 5 page size 100, 500, 1000, 2000, 5000
	for(j = 0; j < 5; j++){

		printf("\nthe page size is %d\n", size[j]);
		fp[0] = fopen("workload7", "r");
		fp[1] = fopen("workload8", "r");
		fp[2] = fopen("workload9", "r");
		fp[3] = fopen("workload10", "r");
		fp[4] = fopen("workload11", "r");
		fp[5] = fopen("workload12", "r");

		int c = 0; //save the vpage id 
		int count; //the vpage number
		int i;

		//total read 6 workdloads
		for(i = 0; i < 6; i++){
			count = 0;
			miss = 0;
			offset = 0;

			//read the workloads data until end of file
			while(fscanf(fp[i], "%d", &c) != EOF){

				if(clockfind(page, c, size[j]) == FALSE){
					clock(page, c, size[j]);
					miss++;
				}

				count++;

				//order to qiuck,just read the 10000 vpages
				if(count >= 10000){
					break;
				}
			}

			//calculate the miss page rate 
			double missrate = miss * 1.0 / count * 100;
			printf("workload%2d  Count is:%d  missNum is:%6d  missRate is:%f%%\n", i + 7, count, miss, missrate);
			fclose(fp[i]);  //close the file pointer
		}

	}

	printf("\n***************lru***************");
	// total 5 page size 100, 500, 1000, 2000, 5000
	for(j = 0; j < 5; j++){

		printf("\nthe page size is %d\n", size[j]);
		fp[0] = fopen("workload7", "r");
		fp[1] = fopen("workload8", "r");
		fp[2] = fopen("workload9", "r");
		fp[3] = fopen("workload10", "r");
		fp[4] = fopen("workload11", "r");
		fp[5] = fopen("workload12", "r");

		int c = 0;	//save the vpage id 
		int count; //the vpage number
		int i;

		//total read 6 workdloads
		for(i = 0; i < 6; i++){
			count = 0;
			miss = 0;
			offset = 0;

			//read the workloads data until end of file
			while(fscanf(fp[i], "%d", &c) != EOF){

				timeflag++;
				if(lrufind(page, c, size[j]) == FALSE){
					lru(page, c, size[j]);
					miss++;
				}

				count++;
				
				//order to qiuck,just read the 10000 vpages
				if(count >= 10000){
					break;
				}
			}
			//calculate the miss page rate 
			double missrate = miss * 1.0 / count * 100;
			printf("workload%2d  Count is:%d  missNum is:%6d  missRate is:%f%%\n", i + 7, count, miss, missrate);
			fclose(fp[i]); //close the file pointer
		}
	}


	return 0;
}

















