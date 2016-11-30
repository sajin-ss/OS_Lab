/* Pgm 8
   Diner
   SAJIN SS-052-s7
   30/09/2016
*/

#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <semaphore.h>

using namespace std;

class dining
{
public:
	int n;
	sem_t chopsticks[50];
	pthread_t philosophers[10];
	int res;

dining()
{}

	void read()
	{
		cout << "\n Enter no of philosophers : ";
		cin >> n;
	}

	void die(string err){
		cout<<err<<" error" <<endl;
		exit(1);
	}

	void sem_start()
	{
		for (int i = 0; i < n; ++i)
		{
			res = sem_init(&chopsticks[i], 0, 0);
			if (res == -1)
				die("stopped at semaphore initialize");
		}
	}

	void static *eat(void *arg)
	{
		long num = (long) arg;

		sem_wait(&chopsticks[num]);
		sem_wait(&chopsticks[(num+1)&n]);
		cout << num << " eating. ";
		sleep(2);
		cout << num << " finished eating. ";
		sem_post(&chopsticks[num]);
		sem_post(&chopsticks[(num+1)&n]);
	}

	void start()
	{
		for (int i = 0; i < n; ++i)
		{
                        long j=i;
			res = pthread_create(&philosophers[i], NULL, eat, (void*)j);
			if (res != 0)
				die(" at thread creation");
			sem_post(&chopsticks[i]);
		}
	}

	void join_thread()
	{
		for (int i = 0; i < n; ++i)
		{
			res = pthread_join(philosophers[i], NULL);
			if (res!=0)
				die("at semaphore join");
		}
	}

	void sem_kill()
	{
		for (int i = 0; i < n; ++i)
		{
			res = sem_destroy(&chopsticks[i]);
			if (res==-1)
				die("at semaphore destruction");
		}
	}


};


int main()
{
	dining d;

	d.read();
	d.sem_start();
	d.start();
	d.join_thread();
	d.sem_kill();

	return 0;
}
