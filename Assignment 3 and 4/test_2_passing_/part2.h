#ifndef PART_2
#define PART_2
#include <semaphore.h>


/**
 * Do any initial setup work in this function.
 * numStations: Total number of stations. Will be >= 5
 * maxNumPeople: The maximum number of people in a train
 */
void initializeP2(int numTrains, int numStations);
/**
 * Print data in the format described in part 5
 */
void * goingFromToP2(void * user_data);


void * startP2();

struct train{
	int ID;
	int stations;
	int* peopleInLine;
	int* peopleInside;
	int total_peeps;
	int max_peeps;
	int current_station;
	sem_t* semInline;
    sem_t* semInside;
    
};

#endif