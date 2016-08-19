/* Eg. 2
   SAJIN - 052 - s7
   PIPE PROCESSES 
   04/07/2015
*/

#include<iostream>
#include<sys/types.h>
#include<unistd.h>
#include<string>

using namespace std;

int isPrime(int);

int main()
{

   int ppid, cpid;
   int wr[100], rd[100], n;
   int pipefd[2];
   
   if (pipe(pipefd) == -1){
      cout << "Error. pipe not formed";
      return -1;
   }

   cout << "\n Enter numbers: ";
   cin >> n;
   
   for (int i=0; i<n; ++i)
      cin >> wr[i];
   
   cpid = fork();
   if (cpid == 0)
   {
      close(pipefd[1]);
      read(pipefd[0], rd, sizeof(wr));
      cout << "data received \n";
      for (int i=0; i<n; ++i)
         if (isPrime(rd[i]))
            cout << rd[i] << " ";
   }
   else
   {
      close(pipefd[0]);
      write(pipefd[1], wr, sizeof(wr));
   }
   cout << endl;
 return 0;
}

int isPrime(int rd){
       if (rd==1)
         return 0;
        for (int j=2; j<=rd/2; ++j)
        {
          if (rd%j==0)
             return 0;
        }
  return 1;
}



/*
OUTPUT
------
>>>
 Enter numbers: 4
3 2 1 3

data received 
3 2 3 



>>>
 Enter numbers: 6
1 2 3 4 5 6

data received 
2 3 5 



>>>>
 Enter numbers: 9
 2 3 5 7 9 11 13 15 17

data received 
2 3 5 7 11 13 17 

*/

