/* Pgm 9
   S7-052-SAJIN
   FTP CLIENT
   19/10/2016
*/

#include "iostream"
#include "sys/types.h"
#include "string.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "unistd.h"
#include "stdlib.h"
#include "fstream"

#define LISTENQ 5

using namespace std;

int main(int argc, char const *argv[])
{

   int sockfd;
   char fname[25];
   int len;
   struct sockaddr_in servaddr, cliaddr;
   
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   bzero(&servaddr, sizeof(servaddr));
   servaddr.sin_family = AF_INET;
   servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
   servaddr.sin_port = htons (atoi(argv[1]));
   
   inet_pton(AF_INET, argv[1], &servaddr);
   connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
   
   char buffer[200];
   ifstream fp("myFile.txt");
   
   while (!fp.eof())
   {
      fp.getline(buffer, 100);
      write(sockfd, buffer, 100);
   }
   buffer[0] = '\0';
   write(sockfd, buffer, 100);
   fp.close();
   cout << "\n File sent \n";
   
   return 0;
}


/* OUTPUT

myFile.txt
----------
a b c d e
  f g h i
    j k l
      m n
        o
        p
      q r
    s t u
  v w x y
z
1 2 3 4 
5 6 7
8 9
0


./serverFTP 23456
File received. saved as gotFile.txt


./clientFTP 23456

File sent


gotFile.txt
----------
a b c d e
  f g h i
    j k l
      m n
        o
        p
      q r
    s t u
  v w x y
z
1 2 3 4 
5 6 7
8 9
0

*/
