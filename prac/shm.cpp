#include <iostream>
#include <stdlib.h>

#include <unistd.h>

#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>

#include <sys/wait.h>


using namespace std;


void error(string msg)
{
	cout << endl << msg << endl;
	exit(0);
}
int main(int argc, char const *argv[])
{

	int shmid, num;

	if ((shmid = shmget(IPC_PRIVATE, sizeof(int), 0666)) < 0)
		error("Error creating shared memory");

	int cid = fork();
	if (cid!=0)
	{
		wait(NULL);

		int *b = (int *) shmat(shmid, NULL, 0);
		int c = b[0];
		cout << c;
	}
	else
	{

		int *a = (int *) shmat(shmid, NULL, 0);
		cin >> num;
		a[0] = num + 9;
	}
	

	cout << endl;
	return 0;
}