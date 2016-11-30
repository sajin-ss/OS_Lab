#include <iostream>
#include <stdlib.h>

#include <semaphore.h>
#include <pthread.h>

#include <unistd.h>

using namespace std;

sem_t chopsticks[20];
pthread_t philosophers[20];
int n;


void *eat(void *arg)
{
	long j = (long ) arg;

	sem_wait(&chopsticks[j]);
	sem_wait(&chopsticks[(j+1)%n]);
	cout << "\nPhil " << j << " eating ";
	sleep(1	);
	cout << "\nPhil " << j << " finished eating ";
	sem_post(&chopsticks[j]);
	sem_post(&chopsticks[(j+1)%n]);
}

int main(int argc, char const *argv[])
{
	
	cout << "n : ";
	cin >> n;

	for (int i = 0; i < n; ++i)
	{
		int k = sem_init(&chopsticks[i], 0, 0);
		if (k==-1)
			return 0;
	}

	for (int i = 0; i < n; ++i)
	{
		long j = i;
		int k = pthread_create(&philosophers[i], NULL, eat, (void *) j);
		if (k!=0)
			return 0;
		sem_post(&chopsticks[i]);
	}

	for (int i = 0; i < n; ++i)
	{
		int res = pthread_join(philosophers[i], NULL);

		if (res!=0)
			return 0;
	}

	for (int i = 0; i < n; ++i)
	{
		sem_destroy(&chopsticks[i]);
	}
	return 0;
}