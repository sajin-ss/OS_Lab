#include<iostream>
#include<semaphore.h>
#include<pthread.h>
#include<fcntl.h>
#include<stdio.h>
using namespace std;
sem_t *sema,*semb;
void *workera(void *args)
{
  while(1)
  {
    sem_wait(sema);
    cout<<"a";
    sem_post(semb);
  }
}
void *workerb(void *args)
{
  while(1)
  {
    sem_wait(semb);
    cout<<"b";
    sem_post(sema);
  }
}
int main()
{
  sema = (sem_t*) malloc(sizeof(sem_t));
  semb = (sem_t*) malloc(sizeof(sem_t));
			 
  pthread_t a,b;
  if(sem_init(sema,0,1)==-1)
    {
      cout<<"Error";
    }
  else
    {
      if(sem_init(semb,0,1)==-1)
	{
	  cout<<"Error";
	}
      else
	{
	  pthread_create(&a,NULL,&workera,NULL);
	  pthread_create(&b,NULL,&workerb,NULL);
	  pthread_exit(NULL);
	}
    };
}
