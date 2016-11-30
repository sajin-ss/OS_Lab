#include <iostream>

#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#include <sys/shm.h>

using namespace std;

int main(int argc, char const *argv[])
{

	int shmid;

	if ( (shmid = shmget(IPC_PRIVATE, 3*sizeof(int), 0666)) == -1)
		return 0;


	int pid = fork();
	if (pid)
	{
		wait(NULL);
		int *cptr = (int *) shmat(shmid, NULL, 0);

			if (cptr < 0)
				return 0;
			cout << endl << endl << " Num 1 is : " << cptr[0];
			cout << endl << endl << " Num 2 is : " << cptr[1];
			cout << "\nSum is  : " << cptr[2];
			shmdt(cptr);
			
	}
	else
	{
		int pid2 = fork();
		if (pid2)
		{
			wait(NULL);
			int *cptr = (int *) shmat(shmid, NULL, 0);

			if (cptr < 0)
				return 0;
			cout << endl << endl << " Num 1 is : " << cptr[0];
			cout << "\nEnter num 2 : ";
			cin >> cptr[1];
			cptr[2] = cptr[1] + cptr[0];
			shmdt(cptr);

		}
		else
		{
			int *cptr = (int *) shmat(shmid, NULL, 0);

			if (cptr < 0)
				return 0;

			cout << "\n Enter num 1 : ";
			cin >> cptr[0];

			shmdt(cptr);
		}
	}
	
	return 0;
}