/**
  * Joshua Osborne
  * CSC 345 001
  * 9 May 2017
  * Dining philosophers problem
  * 
  * This code exercises thread synchronization with several philosophers
  * who eat and think. Deadlock is prevented by forcing the even numbered
  * philosophers to pick up the chopstick ahead of them first and 
  * forcing odd numbered philosophers to pick up the chopstick matching their
  * number first.
  * This way, if a philosopher does not get his first chopstick, he won't 
  * pick up the second one, allowing the next philosopher to eat,
  * preventing deadlock. This is an asymmetric solution.
  * The threads seem to move very quickly; usually a philosopher will eat,
  * immediately put down his chopsticks and pick them up again to eat,
  * but a sleep() can be thrown into the thread function to jostle things
  * around.
  */
#include <stdio.h>
#include <string.h>
#include <pthread.h>

//pthread array must be statically defined using a constant
//the code most likely won't use all the mutexes
//if more than 50 philosophers are desired, expand this number
//but that's a lot of thinkers
#define MAX_PHILOSOPHERS 50
static pthread_mutex_t chopsticks[MAX_PHILOSOPHERS];
//default values for global variables
int philCount = 8;
int eatCount = 4;
void *dine();

int main(int argc, char *argv[] )
{
	if (argc == 3)
	{
		//parse arguments from command line
		eatCount = atoi(argv[1]);
		philCount = atoi(argv[2]);
	}
	else
	{
		printf("You have not the correct number of arguments.\n");
		printf("You must construct a statement with 3 arguments.\n");
		printf("Defaulting to 8 philosophers eating 4 times.\n");
	}
	//Initialize variables
	pthread_t philosophers[philCount];
	int indexArray[philCount];
	//pthread_mutex_t chopsticks[philCount];
	for (int i = 0; i < philCount; i++)
	{
		//Put philosopher threads and chopstick mutexes into arrays
		//Unique index pointers for each philosopher
		indexArray[i] = i;
		pthread_mutex_init(&chopsticks[i], NULL);
		pthread_create(&philosophers[i], NULL, &dine, &indexArray[i]); 
	}
	for (int i = 0; i < philCount; i++)
	{
		//done with threads, finish
		pthread_join( philosophers[i], NULL);
	}
	//only proceed once all threads are completed



	return 0;
}

void *dine(int* n)
{
	//n identifies which philosopher this thread is
	//if philosopher is even, grab right one first then left
	//odd philosophers grab left first
	for (int j = 0; j < eatCount; j++)
	{
		//Sleep function used to test whether threads were in fact running concurrently
		//sleep(1);
		printf("Philosopher %d is thinking... \n", *n);
		if (*n % 2 == 0)
		{
			//philosopher is even
			//grab chopstick to "left" first
			//assuming counting clockwise is up,
			//right chopstick is same
			//left chopstick is one up

			//printf("Philosopher %d goes for chopstick %d... \n", *n, (*n + 1) % philCount);
			//Lock up the chopsticks for use
			pthread_mutex_lock( &chopsticks[ (*n + 1) % philCount ]);
			//printf("Philosopher %d wants to eat... \n", *n);
			pthread_mutex_lock( &chopsticks[ *n ]);
			printf("Philosopher %d is eating... \n", *n);
			//sleep(1);
			//Done eating, unlock chopsticks
			pthread_mutex_unlock( &chopsticks[ *n ]);
			pthread_mutex_unlock( &chopsticks[ (*n + 1) % philCount ]);
		}
		else
		{
			//Odd numbered philosopher, different order of picking up chopsticks
			pthread_mutex_lock( &chopsticks[ *n ]);
			//printf("Philosopher %d wants to eat... \n", *n);
			pthread_mutex_lock( &chopsticks[ (*n + 1) % philCount ]);
			printf("Philosopher %d is eating... \n", *n);
			//sleep(1);
			pthread_mutex_unlock( &chopsticks[ (*n + 1) % philCount ]);
			pthread_mutex_unlock( &chopsticks[ *n ]);
		}

		//printf("Philosopher %d puts down the chopsticks... \n", *n);
		//sleep(1);
	}
}

