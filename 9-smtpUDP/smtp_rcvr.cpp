/* Pgm 09
   SMTP using UDP receiver
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

  int udpSocket, nBytes, i;
  char buffer[1024];

  struct sockaddr_in servaddr, cliaddr;
  struct sockaddr_storage servsto;
  socklen_t addr_size, cli_size;
  

  
  udpSocket = socket(PF_INET, SOCK_DGRAM, 0);
  
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(atoi(argv[1]));
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  
  memset(servaddr.sin_zero, '\0', sizeof(servaddr.sin_zero));
  bind(udpSocket, (struct sockaddr *) &servaddr, sizeof(servaddr));

  addr_size = sizeof(servsto);
  
  struct mail m;
  nBytes = recvfrom(udpSocket, &m, sizeof(m), 0, (struct sockaddr *) &servsto, &addr_size);
  cout <<  "FROM : " << m.from << endl;
  cout << "TO : " << m.to << endl;
  cout <<"SUBJECT : " << m.subject << endl;
  cout << "CONTENT : " << m.content << endl;
  
  bzero(buffer, 255);
  strcpy(buffer, "Received");
  sendto(udpSocket, buffer, 9, 0, (struct sockaddr *) &servsto, addr_size);

  return 0;
}


/* OUTPUT

FROM : Itachi
TO : Tobi
SUBJECT : Your true identity
CONTENT : tobitobitobi . I can see obito within you

*/
