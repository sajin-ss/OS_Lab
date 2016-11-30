/* Pgm 6
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

//struct dining
//{
//public:
	int n;
	sem_t chopsticks[50];
	pthread_t philosophers[10];
	int res;
//}d;
//dining()
//{}


	void die(string err){
		cout<<err<<" error" <<endl;
		exit(1);
	}


	void *eat(void *arg)
	{
		long num = (long) arg;

		sem_wait(&chopsticks[num]);
		sem_wait(&chopsticks[(num+1)%n]);
		cout << num << " eating " << endl;
		sleep(1);
		cout << num << " finished eating " << endl;
		sem_post(&chopsticks[num]);
		sem_post(&chopsticks[(num+1)%n]);
	}


int main()
{
		cout << "\n Enter no of philosophers : ";
		cin >> n;


		for (int i = 0; i < n; ++i)
		{
			res = sem_init(&chopsticks[i], 0, 0);
			if (res == -1)
				die("stopped at semaphore initialize");
		}


		for (int i = 0; i < n; ++i)
		{
                        long j=i;
			res = pthread_create(&philosophers[i], NULL, eat, (void*)j);
			if (res != 0)
				die(" at thread creation");
			sem_post(&chopsticks[i]);
		}

		for (int i = 0; i < n; ++i)
		{
			res = pthread_join(philosophers[i], NULL);
			if (res!=0)
				die("at semaphore join");
		}

		for (int i = 0; i < n; ++i)
		{
			res = sem_destroy(&chopsticks[i]);
			if (res==-1)
				die("at semaphore destruction");
		}




	return 0;
}


/* OUTPUT


 Enter no of philosophers : 4
0 eating 
0 finished eating 
3 eating 
3 finished eating 
2 eating 
2 finished eating 
1 eating 
1 finished eating 

 Enter no of philosophers : 7
0 eating 
2 eating 
5 eating 
0 finished eating 
2 finished eating 
5 finished eating 
1 eating 
4 eating 
6 eating 
1 finished eating 
4 finished eating 
6 finished eating 
3 eating 
3 finished eating 

*/
