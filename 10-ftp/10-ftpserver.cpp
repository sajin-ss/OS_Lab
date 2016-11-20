/* Pgm 9
   S7-052-SAJIN
   FTP-server
   19/10/2016
*/



#include <iostream>
#include <sys/types.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fstream>
#include <stdlib.h>

#define LISTENQ 5


using namespace std;

int main(int argc, char const *argv[])
{
   int fd, sockfd, listenfd, connfd;
   pid_t childpid;
   
   socklen_t client;
   struct sockaddr_in servaddr, cliaddr;
   
   listenfd = socket(AF_INET, SOCK_STREAM, 0);
   
   bzero(&servaddr, sizeof(servaddr));
   
   servaddr.sin_family = AF_INET;
   servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
   servaddr.sin_port = htons(atoi(argv[1]));
   
   bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
   listen(listenfd, LISTENQ);
   client = sizeof(cliaddr);
   connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &client);
   
   char buffer[100];
   ofstream fp;
   fp.open("gotFile.txt");

  
   while(1)
   {
      read(connfd, buffer, 100);
      if (buffer[0] == '\0')
         break;
         fp << buffer << endl;
   }
   fp.close();
   cout << "\n File received. saved as gotFile.txt.\n";
   
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


