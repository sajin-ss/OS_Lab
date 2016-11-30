#include <iostream>

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <sys/shm.h>


using namespace std;

class Matrix
{
public:

	int m, n, A[100][100];

	Matrix()
	{}
	Matrix(int x, int y)
	{
		m = x;
		n = y;
	}

	void read()
	{
		cout << "\n Enter m, n : ";
		cin >> m >> n;

		for (int i = 0; i < m; ++i)
			for (int j = 0; j < n; ++j)
				cin >> A[i][j];
	}

	void disp()
	{
		for (int i = 0; i < m; ++i)
		{
			cout << endl;
			for (int j = 0; j < n; ++j)
				cout << A[i][j] << "  ";
		}
	}

	Matrix add(Matrix b)
	{
		Matrix S(m, n);

		for (int i = 0; i < m; ++i)
		{
			for (int j = 0; j < n; ++j)
			{
				S.A[i][j] = A[i][j] + b.A[i][j]; 
			}
		}

		return S;
	}
	
};
int main(int argc, char const *argv[])
{
	int shmid;

	if ( (shmid = shmget(IPC_PRIVATE, 3*sizeof(Matrix), 0666)) < 0 )
	{
		cout << "Err";
		return 0;
	}

	int cid = fork();

	if (cid)
	{
		wait(NULL);
		int ccid =fork();

		if (ccid)
		{

			wait(NULL);

			Matrix *cptr = (Matrix *) shmat(shmid, NULL, 0);
			if (cptr < 0)
			return 0;

			cptr[0].disp();
			cptr[1].disp();
			cptr[2].disp();

		}
		else
		{
			Matrix *cptr = (Matrix *) shmat(shmid, NULL, 0);

if (cptr < 0)
			return 0;

			Matrix m3 = cptr[0].add(cptr[1]);
			cptr[2] = m3;

			Matrix a = cptr[0];
		}

	}
	else
	{
		Matrix *cptr = (Matrix *) shmat(shmid, NULL, 0 );

		if (cptr < 0)
			return 0;
		Matrix m1;
		m1.read();
		Matrix m2;
		m2.read();

		cptr[0] = m1;
		cptr[1] = m2;
	}
	
	return 0;
}
