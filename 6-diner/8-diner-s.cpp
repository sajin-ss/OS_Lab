/* Pgm 8
   Diner
   SAJIN-052-s7
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

struct dining
{
//public:
	int n;
	sem_t chopsticks[50];
	pthread_t philosophers[10];
	int res;
}d;
//dining()
//{}

	void read(struct dining &d)
	{
		cout << "\n Enter no of philosophers : ";
		cin >> d.n;
	}

	void die(string err){
		cout<<err<<" error" <<endl;
		exit(1);
	}

	void sem_start(struct dining &d)
	{
		for (int i = 0; i < d.n; ++i)
		{
			d.res = sem_init(&d.chopsticks[i], 0, 0);
			if (d.res == -1)
				die("stopped at semaphore initialize");
		}
	}

	void *eat(void *arg)
	{
		long num = (long) arg;

		sem_wait(&d.chopsticks[num]);
		sem_wait(&d.chopsticks[(num+1)%d.n]);
		cout << num << " eating. ";
		sleep(2);
		cout << num << " finished eating. ";
		sem_post(&d.chopsticks[num]);
		sem_post(&d.chopsticks[(num+1)%d.n]);
	}

	void start(struct dining &d)
	{
		for (int i = 0; i < d.n; ++i)
		{
                        long j=i;
			d.res = pthread_create(&d.philosophers[i], NULL, eat, (void*)j);
			if (d.res != 0)
				die(" at thread creation");
			sem_post(&d.chopsticks[i]);
		}
	}

	void join_thread(struct dining &d)
	{
		for (int i = 0; i < d.n; ++i)
		{
			d.res = pthread_join(d.philosophers[i], NULL);
			if (d.res!=0)
				die("at semaphore join");
		}
	}

	void sem_kill(struct dining &d)
	{
		for (int i = 0; i < d.n; ++i)
		{
			d.res = sem_destroy(&d.chopsticks[i]);
			if (d.res==-1)
				die("at semaphore destruction");
		}
	}




//	dining d;

int main()
{

	read(d);
	sem_start(d);
	start(d);
	join_thread(d);
	sem_kill(d);

	return 0;
}
