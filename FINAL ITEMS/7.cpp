/*
  Pgm No 7
  Banker's Algorithm
  SAJIN-052-S7
  17/09/2016
*/


#include <iostream>
#include <fstream>
#include <set>
#include <queue>

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
      {
        cout << "\n Request exceeds Need \n";
         return false;
      }
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

queue<int> processOrder;
queue<int> emp;
set<int> done;
set<int> donemp;
    while (!proc.empty())
    {
       for (int i=0; i<n; ++i)
       {
          set<int>::iterator iit;
          if ((iit = proc.find(i)) != proc.end() )
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
                  T_available[j] += allocation[i][j];
                  T_need[i][j] = 0;

                  set<int>::iterator it;
                  if ( ( it = done.find(i) ) == done.end() )
                     processOrder.push(i);
                  done.insert(i);
                  proc.erase(i);
              }
           }
           else
           {
            cout << i << " not possible \n";
           }
         }
       }
       k--;
       if (k==0)
         return false;
    }
    cout << endl;
    while (!processOrder.empty())
    {
       cout << processOrder.front() << "-->";
       processOrder.pop();
    }
    cout << endl;
   return true;
}

void grantRequest(int m, int n)
{

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
	
	           cout << "\n Granted \n ";
       }
       else
            cout << "\n Cannot be granted \n";

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
		


    int c;
    cout << endl << "MENU" << endl << "1. Status\n2. Request\n3. Exit\n";
     do
     {
     cin >> c;
     if (c==1)
     {
        cout << "\nAvailable\n";
	dispV(available, m);
        cout << "\nMax Reqd";
	dispM(max_res, n, m);
        cout << "\nCurrent Allocation";
	dispM(allocation, n, m);
        cout << "\nNeed";
	dispM(need, n, m);
     
     }
     else if (c==2)
     {
        grantRequest(m, n);
     }
     
     }while(c!=3);
     

cout << endl;
return 0;
}



/* OUTPUT

INPUT:

4 3

2 2 2

5 4 2
3 1 3
4 2 4
1 2 1

2 2 1
0 1 0
2 0 2
1 2 0




OUTPUT:

MENU
1. Status
2. Request
3. Exit
1

Available
2  2  2  

Max Reqd
5  4  2  
3  1  3  
4  2  4  
1  2  1  

Current Allocation
2  2  1  
0  1  0  
2  0  2  
1  2  0  

Need
3  2  1  
3  0  3  
2  2  2  
0  0  1  


2

 Enter process num : 0

Enter resource request : 3 2 1

 Cannot be granted 

2

 Enter process num : 0 

Enter resource request : 2 2 1

3-->0-->1-->2-->

 Granted 
 
1

Available
0  0  1  

Max Reqd
5  4  2  
3  1  3  
4  2  4  
1  2  1  

Current Allocation
4  4  2  
0  1  0  
2  0  2  
1  2  0  

Need
1  0  0  
3  0  3  
2  2  2  
0  0  1  

2

 Enter process num : 2

Enter resource request : 0 0 1

 Cannot be granted 

2

 Enter process num : 3

Enter resource request : 0 0 1

3-->0-->1-->2-->

 Granted 
 
2

 Enter process num : 0

Enter resource request : 1 0 0

0-->1-->2-->

 Granted 
 

2

 Enter process num : 1

Enter resource request : 3 0 3

1-->2-->

 Granted 
 1

Available
5  7  3  

Max Reqd
5  4  2  
3  1  3  
4  2  4  
1  2  1  

Current Allocation
0  0  0  
0  0  0  
2  0  2  
0  0  0  

Need
0  0  0  
0  0  0  
2  2  2  
0  0  0  

2

 Enter process num : 2 

Enter resource request : 2 2 2

2-->

 Granted 
 3


*/
