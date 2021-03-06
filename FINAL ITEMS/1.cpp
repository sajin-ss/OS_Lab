#include <iostream>

#include <sys/types.h>
#include <sys/wait.h>

#include <unistd.h>

using namespace std;

	//   p1
	// /    \
	// p2	  p3
 //   / | \    \
 //  p4 p5 p6 


int main(int argc, char const *argv[])
{
	
	int p2id = fork();


	if (p2id)
	{
		wait(NULL);		//p

		int p3id = fork();
		if (p3id)
		{
			wait(NULL);		//p
			cout << "P1";
		}
		else
		{
			// wait(NULL);		//c
			cout << "P3";
		}

		// cout << "\nPARENT\n------";
		// cout << endl << " Process ID -> " << getpid();
		// cout << endl << " Parent ID -> " << getppid();
	}
	else
	{
		// wait(NULL);		//c
		int p4id = fork();
		if (p4id)
		{
			wait(NULL);		//p
			int p5id = fork();
			if (p5id)
			{
				wait(NULL);		//p
				int p6id = fork();
				if (p6id)
				{
					wait(NULL);		//p
					cout << "P2";
				}
				else
				{
					// wait(NULL);		//c
					cout << "P6";
				}
			}
			else
			{
				// wait(NULL);		//c
				cout << "P5";
			}
		}
		else
		{
			// wait(NULL);		//c
			cout << "P4";
		}
		// cout << "\nCHILD\n-----";
		// cout << endl << " Process ID -> " << getpid();
		// cout << endl << " Parent ID -> " << getppid();
	}

cout << endl;
	return 0;
}