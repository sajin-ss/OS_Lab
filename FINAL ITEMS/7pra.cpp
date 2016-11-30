#include <iostream>
#include <fstream>
#include <set>
#include <queue>

using namespace std;

#define lt 20

int available[lt];
int max_res[lt][lt];
int allocation[lt][lt];
int need[lt][lt];

set<int> working;

void printV(int a[], int m)
{
	cout << endl;
	for (int i = 0; i < m; ++i)
	{
		cout << a[i] << "  ";
	}
}

void printM(int a[][lt], int m, int n)
{
	cout << endl;
	for (int i = 0; i < m; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			cout << a[i][j] << "  ";
		}
		cout << endl;
	}
}


bool checkSafety(int pid, int req[], int m, int n)
{
	//copies
	int t_available[lt];
	int t_allocation[lt][lt];
	int t_need[lt][lt];

	for (int i = 0; i < m; ++i)
		t_available[i] = available[i];

	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < m; ++j)
		{
			t_allocation[i][j] = allocation[i][j];
			t_need[i][j] = need[i][j];
		}
	}

	set<int> proc(working);

	//GRANT REQUEST TEMPORARILY

	queue<int> sequence;

	int over = 1;
	for (int i = 0; i < m; ++i)
	{
		t_available[i] -= req[i];
		t_allocation[pid][i] += req[i];
		t_need[pid][i] -= req[i];
		if (t_need[pid][i] != 0)
			over = 0;
	}
	if (over)
	{
		sequence.push(pid);
		proc.erase(pid);
		for (int i = 0; i < m; ++i)
		{
			t_available[i] += t_allocation[pid][i];
			t_allocation[pid][i] = 0;
		}

		
	}

int k = n*n;




	while(!proc.empty())
	{
// 		for (set<int>::iterator i = proc.begin(); i != proc.end(); ++i)
// {
// 	cout << *i << ",";
// }
// cout << endl;
		for (int i = 0; i < n; ++i)
		{
			set<int>::iterator it;
			if ((it = proc.find(i)) != proc.end())
			{
				int ok = 1;
				for (int j = 0; j < m; ++j)
				{
					if (t_need[i][j] > t_available[j])
						ok = 0;
				}

				if (ok)
				{
					for (int j = 0; j < m; ++j)
					{
						t_available[j] += t_allocation[i][j];
						t_allocation[i][j] = 0;
						t_need[i][j] = 0;
					}

					proc.erase(i);
					sequence.push(i);
		// cout <<endl<< i << "DONE DONE DONE";

           		 // cout << i << "  possible \n";

				}
				else
				{
           		 // cout << i << " not possible \n\n";
           		 // printV(t_available, m);
           		 // printM(t_need, n, m);
           		 // cout << endl;
           		}
			}
			// else
				// cout << endl << i << " NOT FOUND";
		}

		k--;
		if (k==0)
			return false;
	}

	cout << endl << "Request safe : --Sequence order =>";
	while(!sequence.empty())
	{
		int p = sequence.front();
		cout << p << " -> ";
		sequence.pop();
	}

	return true;
}

void request(int m, int n)
{
	int req[lt], pid;
	cout << "\nEnter process # : ";
	cin >> pid;
	cout << "\nEnter request : ";
	for (int i = 0; i < m; ++i)
		cin >> req[i];

	for (int i = 0; i < m; ++i)
	{
		if (req[i] > need[pid][i])
		{
			cout << "\n! Request exceeds Need !";
			return;
		}
		if (req[i] > available[i])
		{
			cout << "\n! Request exceeds Available !";
			return;
		}
	}

	if (checkSafety(pid, req, m, n))
	{
		int over = 1;
		for (int i = 0; i < m; ++i)
		{
			available[i] -= req[i];
			allocation[pid][i] += req[i];
			need[pid][i] -= req[i];

			if (need[pid][i] != 0)
				over = 0;
		}

		cout << "\n Request Granted ";

		if (over)
		{
			working.erase(pid);
			cout << "\n Process " << pid << " completed ";

			for (int i = 0; i < m; ++i)
			{
				available[i] += max_res[pid][i];
				allocation[pid][i] = 0;
			}
		}
	}
	else
	{
		cout << "\n ! Request not Safe. Can't be granted !";
		return;
	}
}

int main(int argc, char const *argv[])
{
	
	if (argc < 2)
		return 0;

	int m, n;	//n -> processes ; m->resources

	fstream fin(argv[1], ios::in);
	fin >> n >> m;

	for (int i = 0; i < n; ++i)
		working.insert(i);

	for (int i = 0; i < m; ++i)
		fin >> available[i];

	for (int i = 0; i < n; ++i)
		for (int j = 0; j < m; ++j)
			fin >> max_res[i][j];

	for (int i = 0; i < n; ++i)
		for (int j = 0; j < m; ++j)
		{
			fin >> allocation[i][j];
			need[i][j] = max_res[i][j] - allocation[i][j];
		}	


	cout << "\n MENU \n1. Status\n2. Request\n3. Exit";

	int ch;
	while(1)
	{
		cout << "\n:";
		cin >> ch;

		if (ch==1)
		{
			cout << "\nAvailable";
			printV(available, m);
			cout << "\nMax Resources";
			printM(max_res, n, m);
			cout << "\nAllocation";
			printM(allocation, n, m);
			cout << "\nNeed";
			printM(need, n, m);
		}
		else if (ch==2)
		{
			request(m, n);
		}
		else
		{
			break;
		}

	}

	return 0;
}