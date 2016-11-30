/* Pgm. 5
   SAJIN - 052 - s7
   SOCKET SERVER
   29/07/2015
*/

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
using namespace std;


void error(const char *msg)
{
   cout << endl << msg << endl;
   exit(1);
}



int main(int argc, char *argv[])
{
   int sockfd, newsockfd, portno;
   socklen_t clilen;
   char buffer[256];
   struct sockaddr_in serv_addr, cli_addr;
   int n;
   
   if (argc < 2)
     error("Port Number not provided ! ");
     
   sockfd = socket(AF_LOCAL, SOCK_STREAM, 0);
   if (sockfd < 0)
     error("Error opening socket ! ");
   
   bzero((char *) &serv_addr, sizeof(serv_addr));
   portno = atoi(argv[1]);
   serv_addr.sin_family = AF_LOCAL;           //AF_INET
   serv_addr.sin_addr.s_addr = INADDR_ANY;
   serv_addr.sin_port = htons(portno);

   if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
      error("ERROR on Binding ");
   
   listen(sockfd, 5);
   clilen = sizeof(cli_addr);
   newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
   if (newsockfd < 0)
      error("ERROR on accept");



while(1)
{
   bzero(buffer, 256);
   n = read(newsockfd, buffer, 255);
   
   if (n<0)
      error("ERROR reading msg from socket");
   
   cout << "--> " << buffer << endl;
char line[256];   
   
   cin.getline(line, 255);
   n = write(newsockfd, line, sizeof(line));
   
   if (n<0)
      error("ERROR writing msg to socket");
}
   close(newsockfd);
   close(sockfd);
   
   
 return 0;
}


/*
OUTPUT
------
HI
--> 

--> Hello

Bye
--> 


--> ok Bye

^C

*/

