#include <iostream>

#include <sys/types.h>
#include <sys/wait.h>

#include <unistd.h>

using namespace std;


int isPrime(int n)
{
	if (n==1)
		return 0;
	for (int i = 2; i <= n/2; ++i)
	{
		if (n%i==0)
			return 0;
	}
	return 1;
}

int main(int argc, char const *argv[])
{
	int pipefd[2];

	if (pipe(pipefd) == -1)
	{
		cout << "\n Error pipe \n";
		return 0;
	}
	int cid = fork();

	if (cid)
	{
		wait(NULL);

		int b[100], n;

		close(pipefd[1]);
		read(pipefd[0], &n, sizeof(n));

		// close(pipefd[1]);
		read(pipefd[0], &b, sizeof(b));

		cout << endl;
		for (int i = 0; i < n; ++i)
		{
			if (isPrime(b[i]))
				cout << b[i] << "  ";
		}
		cout << endl;
	}
	else
	{
		int a[100], n;
		cout << "\n Enter n : ";
		cin >> n;
		for (int i = 0; i < n; ++i)
		{
			cin >> a[i];
		}

		close(pipefd[0]);
		write(pipefd[1], &n, sizeof(n));

		// close(pipefd[0]);
		write(pipefd[1], &a, sizeof(a));
	}

	return 0;
}