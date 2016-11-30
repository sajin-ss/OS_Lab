#include <iostream>
#include <unistd.h>


#include <semaphore.h>
#include <pthread.h>

using namespace std;


sem_t chopsticks[10];
pthread_t phil[10];
sem_t mutex;
int n;

void *eat(void *arg)
{
	long i = (long) arg;

	sem_wait(&chopsticks[i]);
	sem_wait(&chopsticks[(i+1)%n]);
	sem_wait(&mutex);

	cout << "\nPhil " << i << " eating . ";
	sleep(2);
	cout << "\nPhil " << i << " finished eating . ";

	sem_post(&chopsticks[i]);
	sem_post(&chopsticks[(i+1)%n]);
	sem_post(&mutex);

}

int main(int argc, char const *argv[])
{
	cout << "N : ";
	cin >>  n;

	for (int i = 0; i < n; ++i)
		sem_init(&chopsticks[i], 0, 0);
	sem_init(&mutex, 0, 1);

	for (int i = 0; i < n; ++i)
	{
		long j = i;
		pthread_create(&phil[i], NULL, eat,  (void *) j);

		sem_post(&chopsticks[i]);
	}

	for (int i = 0; i < n; ++i)
	{
		pthread_join(phil[i], 0);
	}

	for (int i = 0; i < n; ++i)
	{
		sem_destroy(&chopsticks[i]);
	}
	return 0;
}