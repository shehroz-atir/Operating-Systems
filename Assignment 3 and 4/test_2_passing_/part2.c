#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include "part2.h"
#include "main.h"

/*
	- DO NOT USE SLEEP FUNCTION
	- Define every helper function in .h file
	- Use Semaphores for synchronization purposes
 */


/**
* Declare semaphores here so that they are available to all functions.
*/
// sem_t* example_semaphore;
const int INTER_ARRIVAL_TIME = 5;
const int NUM_TRAINS = 5;



/**
 * Do any initial setup work in this function. You might want to 
 * initialize your semaphores here. Remember this is C and uses Malloc for memory allocation.
 *
 * numStations: Total number of stations. Will be >= 5. Assume that initially
 * the first train is at station 1, the second at 2 and so on.
 * maxNumPeople: The maximum number of people in a train
 */
 struct train* Train;
 int* peopleInLine1;

 //sem_t* semInline;
 //sem_t* semInside;
 sem_t* mutex;

void initializeP2(int numStations, int maxNumPeople) {
	// example_semaphore = (sem_t*) malloc(sizeof(sem_t)); 
	//semInline=malloc(sizeof(sem_t)*numStations);
	//semInside=malloc(sizeof(sem_t)*numStations);
	mutex=malloc(sizeof(sem_t));

	int i=0;
	int j=0;
	
	sem_init(mutex,0,1);
	peopleInLine1=malloc(sizeof(int)*numStations);

	//initialising the  array of trains
	Train=malloc(sizeof(struct train)*NUM_TRAINS);
	for(i=0;i<NUM_TRAINS;i++)
	{
		Train[i].ID=i;
		Train[i].stations=numStations;
		Train[i].total_peeps=0;
		Train[i].max_peeps=maxNumPeople;
		Train[i].current_station=i;
		Train[i].peopleInside=malloc(sizeof(int)*numStations);
		Train[i].peopleInLine=malloc(sizeof(int)*numStations);
		Train[i].semInline=malloc(sizeof(sem_t)*numStations);
	    Train[i].semInside=malloc(sizeof(sem_t)*numStations);
		for (j=0;j<numStations;j++)
		{
			Train[i].peopleInside[j]=0;
			Train[i].peopleInLine[j]=0;
			sem_init(Train[i].semInline,0,0);
	        sem_init(Train[i].semInside,0,0);
	
			
		}
	}
}

/**
	This function is called by each user.

 * Print in the following format:
 * If a user borads on train 0, from station 0 to station 1, and another boards
 * train 2 from station 2 to station 4, then the output will be
 * 0 0 1
 * 2 2 4
 */
/*struct train shortest() //function to find the nearest train
{
	int shortest=Train[0].current_station;
	struct train nearest_train;
	//nearest_train=malloc(sizeof(struct train));
	nearest_train=Train[0];
	int curr;
	int i;	
	//printf("%d\n",shortest);
	for (i=1;i<NUM_TRAINS;i++)
	{ 
		if (Train[i].current_station< shortest)
		{
			shortest= Train[i].current_station;
			nearest_train=Train[i];
			printf("current%d\n",Train[i].current_station );
			printf("shortest%d\n",shortest);
		}
	}
	printf("shortest is %d\n",shortest);
	return nearest_train;
    
}*/

void * goingFromToP2(void * user_data) {

	//create a user
	struct argument* user= (struct argument *) user_data;
	int src=user->from;
	int dest=user->to;
	int u_id=user->id;
	//printf("%s\n","in user side");
	

    int i=0;
	int c=0;
	sem_wait(mutex);
	for (i=0;i<NUM_TRAINS;i++)
	{
			Train[i].peopleInLine[src]++;
			//printf("%d\n",i);
	}
	sem_post(mutex);


	//for choosing a train
	int train_chosen;
	for (i=0;i<NUM_TRAINS;i++)
	{   //printf("%s\n","waiting for train");
		if(Train[i].current_station==src) //choosing the train which is at the station
		{
			
			//printf("train is: ");
			//printf("%d\n",i);
			train_chosen=i;
			sem_wait(&Train[i].semInline[src]); //waiting in the inline sem of that train
			//printf("user %d has gotten into train %d at station %d gets off at %d\n",u_id,Train[i].ID,src,dest);
            
           
			

			//sem_wait(&Train[i].semInside[dest]);

			//sem_wait(mutex);
			//Train[i].peopleInside[dest]--;
			//sem_post(mutex);

			break;
			
		}
	}

	sem_wait(mutex);
	//int j=0;
	Train[train_chosen].peopleInside[dest]++;
	peopleInLine1[src]--;
	for (i=0;i<NUM_TRAINS;i++)
			{
				Train[i].peopleInLine[src]--;
				//printf("%s\n","update people in line");
			}
	sem_post(mutex);

	sem_wait(&Train[train_chosen].semInside[dest]);

	sem_wait(mutex);
	Train[train_chosen].peopleInside[dest]--;
	sem_post(mutex);




	

	sem_wait(mutex);
	//printf("output ");
	printf("%d",Train[train_chosen].ID);
	printf("%s"," ");	
	printf("%d",src);
	printf("%s"," ");	
	printf("%d\n",dest);
   // printf("%s"," ");	

	//printf("%d\n",src);
	//printf("%d\n",dest);
	sem_post(mutex);

	



}

void* trainfunc(void *args)
{   
    sem_wait(mutex);
    struct train* Train_func= (struct train*) args;
	//printf("%s\n","in train func");
	sem_post(mutex);

	//sem_post(semInline[])
	while(1)
	{
		int i=0;
		for(i=0;i<Train_func->stations;i++)
		{
			while (Train_func->peopleInside[i] != 0)
			{
				sem_post(&Train_func->semInside[i%Train_func->stations]); //get people off at the station

			}
			while(Train_func->peopleInLine[i] && Train_func->total_peeps < Train_func->max_peeps )
			{
				sem_post(&Train_func->semInline[i]); //board people on train 
			}
		}



		/*if (elev->max_peeps == 0)
	   {
	   		break;
	   }

	   for (i=0;i<elev->floors;i++)
	   {
	   	 count=count+elev->peopleInLine[i];
	   	// printf("%d\n",count );
	   	// printf("%s","total peeps:");
	   	 //printf("%d\n",elev->total_peeps);
	   }

	   if (elev->total_peeps == 0 && count ==0)
	   	break;*/
	}
	
	return;
}


/* Use this function to start threads for your trains */
void * startP2(){

	//sleep(1);

	int i;
	/*pthread_t train_threads[NUM_TRAINS];

	for (i=0;i<5;i++)
	{
		pthread_t t_id;
		pthread_create(&t_id,NULL,trainfunc,(void*)Train);
		train_threads[i]=t_id;
	}*/
	pthread_t* tid;
	tid=malloc(sizeof(pthread_t)*NUM_TRAINS);
	for(i=0;i<5;i++)
	{
		pthread_create(&tid[i],NULL,trainfunc,&Train[i]);
		//printf("thread %d created\n",i);
	}
	

	return;

	

}