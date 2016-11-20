#include <iostream>
#include <fstream>
#include <set>

using namespace std;

#define lt 20


	//Data Structures
	int available[lt];
	int max_res[lt][lt];
	int allocation[lt][lt];
	int need[lt][lt];

	set<int> working;

void dispV(int *a, int n)
{
	for (int i = 0; i < n; ++i)
		cout << *(a+i) << "  ";
	cout << endl;

}
void dispM(int a[][lt], int n, int m)
{
	for (int i = 0; i < n; ++i)
	{
		cout << endl;
		for (int j = 0; j < m; ++j)
		{
			cout << a[i][j] << "  ";
		}
	}
	cout << endl;
}


bool checkSafety(int n, int p, int req[], int m)
{
   for (int i=0; i<m; ++i)
   {
      if (req[i] > need[p][i])
         return false;
   }
   
    set<int> proc(working);
    
	int T_available[lt];
	int T_allocation[lt][lt];
	int T_need[lt][lt];
    
    for (int i=0; i<m; ++i)
       T_available[i] = available[i];
       
    for (int i=0; i<n; ++i)
       for (int j=0; j<m; j++)
       {
          T_allocation[i][j] = allocation[i][j];
          T_need[i][j] = need[i][j];
       }
    
    
	//Grant request temporarily
	for (int i = 0; i < m; ++i)
	{
	   T_available[i] -= req[i];
	   T_need[p][i] -= req[i];
	   T_allocation[p][i] += req[i];
	}

int k = n * n;
    while (!proc.empty())
    {
    
       for (int i=0; i<n; ++i)
       {
          int ok = 1;
          for (int j=0; j<m; ++j)
          {
            if (T_need[i][j] > T_available[j])
               ok = 0;
          }
          
          if (ok)
          {
             for (int j=0; j<m; ++j)
             {
                 T_available[j] += max_res[i][j];
                 T_need[i][j] = 0;
                 proc.erase(i);
             }
          }
       }
       
       k--;
       if (k==0)
         return false;
    }
   return true;
}


int main(int argc, char const *argv[])
{

	ifstream fin(argv[1]);

	int n, m;			// n -> # of processes
	fin >> n >> m;		// m -> # of resources

	for (int i=0; i<m; ++i)
		fin >> available[i];

	for (int i=0; i<n; ++i)
	{
	    working.insert(i);
		for (int j=0; j<m; ++j)
			fin >> max_res[i][j];
	}

	for (int i=0; i<n; ++i)
		for (int j=0; j<m; ++j)
		{
			fin >> allocation[i][j];
			need[i][j] = max_res[i][j] - allocation[i][j];			
		}
		

        cout << "\nAvailable";
	dispV(available, m);
        cout << "\nMax Reqd";
	dispM(max_res, n, m);
        cout << "\nCurrent Allocation";
	dispM(allocation, n, m);
        cout << "\nNeed";
	dispM(need, n, m);

        
        const int M = m;
        int p;
        int req[M];
        
        cout << "\n Enter process num : ";
        cin >> p;
        cout << "\nEnter resource request : ";
        
        for (int i=0; i<m; ++i)
           cin >> req[i];
           
       if ( checkSafety(n, p, req, m))
       {
       
       int over = 1;
	for (int i = 0; i < m; ++i)
	{
	   available[i] -= req[i];
	   need[p][i] -= req[i];
	   allocation[p][i] += req[i];
	   
	   if (need[p][i]!=0)
	     over = 0;
	}
	
	if (over)
	{
	  working.erase(p);
	  
	  for (int i=0; i<m; ++i)
	  {
	     available[i] += max_res[p][i];
	     allocation[p][i] = 0;
	  }
	}
	
	
       }


	cout << endl << "After allocation \n";
        cout << "\nAvailable";
	dispV(available, m);
        cout << "\nMax Reqd";
	dispM(max_res, n, m);
        cout << "\nCurrent Allocation";
	dispM(allocation, n, m);
        cout << "\nNeed";
	dispM(need, n, m);



cout << endl;
return 0;
}

