/* Pgm. 5
   SAJIN - 052 - s7
   SOCKET CLIENT
   29/07/2016
*/
 
#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>

using namespace std;


void error(const char *msg)
{
   cout << endl << msg << endl;
   exit(1);
}



int main(int argc, char *argv[])
{
   int sockfd, portno;
   //socklen_t clilen;
   char buffer[256];
   struct sockaddr_in serv_addr;
   struct hostent *server;
   int n;
   
   if (argc < 3)
   {  cout << "Usage " << argv[0] << " hostname port\n";
      exit(0);
   }
   portno = atoi(argv[2]);
     
   sockfd = socket(AF_LOCAL, SOCK_STREAM, 0);
   if (sockfd < 0)
     error("Error opening socket ! ");
   
   server = gethostbyname(argv[1]);
   if (server == NULL)
     error("ERROR, no such host");
   
   bzero((char *) &serv_addr, sizeof(serv_addr));
   serv_addr.sin_family = AF_LOCAL;           //AF_INET
   bcopy((char *) server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);
   serv_addr.sin_port = htons(portno);
   serv_addr.sin_addr.s_addr = INADDR_ANY;

   if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
       error("ERROR connecting");
   
//   cout << "\n Enter msg : ";
while(1)
{
   bzero(buffer, 256);
   fgets(buffer, 255, stdin);
   n = write(sockfd, buffer, strlen(buffer));
   if (n<0)
      error("ERROR writing to socket");
   bzero(buffer, 256);
   n = read(sockfd, buffer,255);
   if (n<0)
      error ("ERROR reading from socket");
   cout << "-->" <<  buffer << endl;
}
   close(sockfd);
 return 0;
}


/*
OUTPUT
------
-->HI
Hello
-->

-->Bye
ok Bye
-->
^C

*/

