/* Pgm 09
   SMTP using UDP sender
   52-SAJIN-S7
   04/11/2016
*/

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>

using namespace std;


struct mail
{
   char from[50], to[50], subject[128], content[1024];
};

int main(int argc, char *argv[])
{

  int cliSocket, nBytes;
  char buffer[1024];
  struct sockaddr_in servaddr;
  socklen_t addr_size;
  
  cliSocket = socket(PF_INET, SOCK_DGRAM, 0);
  
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(atoi(argv[1]));
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  
  memset(servaddr.sin_zero, '\0', sizeof(servaddr.sin_zero));
  
  addr_size = sizeof(servaddr);

  struct mail m;
  
  cout <<  "FROM : ";
  cin.getline(m.from, 50);
  cout << "TO : ";
  cin.getline(m.to, 50);
  cout <<"SUBJECT : ";
  cin.getline(m.subject, 1024);
  cout << "CONTENT : ";
  cin.getline(m.content, 1024);
  
  sendto(cliSocket, &m, sizeof(m), 0, (struct sockaddr *) &servaddr, addr_size);

  nBytes = recvfrom(cliSocket, buffer, sizeof(buffer), 0, NULL, NULL);
  cout << buffer;

  return 0;
}


/* OUTPUT

s1351@linux-server:~/s7/9-smtpUDP$ ./send.out 23456
FROM : Itachi
TO : Tobi
SUBJECT : Your true identity
CONTENT : tobitobitobi . I can see obito within you

Received
*/
