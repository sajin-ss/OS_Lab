  /* Pgm. 3
   SAJIN - 052 - s7
   SHARED MEMORY MATRIX 
   21/08/2015
*/

#include <iostream>
#include <unistd.h>
#include <string>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

class matrix
{
   int m,n,A[5][5];
public:

     matrix(int row, int col)
     {
        m=row;
        n=col;
     }
     
     void read()
     {
        cout << "Input elements: \n";
        for (int i=0; i<m; ++i)
            for (int j=0; j<n; ++j)
                cin >> A[i][j];
     }
     
     void display()
     {
     	cout<<"matrix "<<endl;
        for(int i =0;i<m;i++)
        {
            for(int j=0;j<n;j++)
     		cout<<A[i][j]<<'\t';
     	 cout<<endl;
       }
     }
     
     matrix multiply(matrix m2)
     {
         matrix m3(m, m2.n);
         if (n != m2.m)
            cout << "matrix not compatible \n";
         else
         {
            for (int i=0; i<m; ++i)
               for (int j=0; j<m2.n; ++j)
               {
                   m3.A[i][j] = 0;
                   for (int k=0; k<n; ++k)
                       m3.A[i][j] += A[i][k] * m2.A[k][j];
               }
         }
         return m3;
     }

};

int main()
{
   int shmid;
   
    if ((shmid = shmget(IPC_PRIVATE, 3*sizeof(matrix), 0666)) < 0)
   {
      cout << "\nError in creating shmid\n";
      return 0;
   }

   int cid = fork();
   if (cid)
   {
      wait(NULL);
      
      int cid2 = fork();

      if (cid2)
      {
         //PARENT

         matrix *shmptr = (matrix*) shmat(shmid, NULL, 0);
         if (shmptr <0)
         {
          cout << "Noooooooooo"<<endl;
          return 0;
         } 

      
         //parent displays output after children do matrix operations
         matrix m1 = *shmptr;
         m1.display();
         matrix m2 = *(shmptr+1);
         m2.display();
         matrix m3 = *(shmptr+2);
         m3.display();
      }
      else
      {
         //CHILD 2 - does the multiply
         matrix *ccptr = (matrix*) shmat(shmid, NULL, 0);
         
         matrix m1 = *ccptr;
         matrix m2 = *(ccptr+1);
         
         matrix m3 = m1.multiply(m2);
         *(ccptr+2) = m3;
         
      }
      
      
   }
   else
   {
      //CHILD 1 - does the read
      
      matrix *cptr = (matrix*) shmat(shmid, NULL, 0);
      
      int r, c;
      cout << "\nEnter size of matrix 1 : ";
      cin >> r >> c;
      
      matrix m1(r, c);
      m1.read();
      *cptr = m1;

      cout << "\nEnter size of matrix 2 : ";
      cin >> r >> c;
      
      matrix m2(r, c);
      m2.read();
      *(cptr+1) = m2;
   }

 cout << endl;
 return 0;
}



/*
OUTPUT
------

Enter size of matrix 1 : 2 3
Input elements: 
1 2 3 4 5 6

Enter size of matrix 2 : 3 2
Input elements: 
2 3 4 5 6 7

matrix 1
1	2	3	
4	5	6	

matrix 2 
2	3	
4	5	
6	7	

product matrix 
28	34	
64	79	


*/

