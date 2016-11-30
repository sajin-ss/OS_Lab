#include <iostream>

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;

void error(string s)
{
	cout << endl << s;
}
int main(int argc, char const *argv[])
{
	int pipefd[2];

	if (pipe(pipefd)==-1)
	{
		error("Pipe not created");
		return 0;
	}

	int pid = fork();

	if (pid)
	{
		wait(NULL);

		int s;
		close(pipefd[1]);
		read(pipefd[0], &s, sizeof(s));
		cout << "Square : " << s * s;
	}
	else
	{
		int c;
		cout << "\nEnter num : ";
		cin >> c;
		close(pipefd[0]);
		write(pipefd[1], &c, sizeof(c));
	}
	return 0;
}