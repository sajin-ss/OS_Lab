#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
using namespace std;
struct mail{
        char from[255],to[255],subject[1024],content[1024];
};
int main(int argc, char **argv){
	int cliSocket, portNum, nBytes;
	char buffer[255];
	struct sockaddr_in servaddr;
	socklen_t addr_size;
	cliSocket=socket(PF_INET,SOCK_DGRAM,0);
	servaddr.sin_family=AF_INET;
	servaddr.sin_port=htons(atoi(argv[1]));
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	memset(servaddr.sin_zero,'\0',sizeof(servaddr.sin_zero));
	addr_size=sizeof(servaddr);
        struct mail m;
        cout<<"FROM: ";
        cin.getline(m.from,255);
        cout<<"TO: ";
        cin.getline(m.to,255);
        cout<<"SUBJECT: ";
        cin.getline(m.subject,1024);
        cout<<"content: ";
        cin.getline(m.content,1024);
	sendto(cliSocket,&m,sizeof(m),0,(struct sockaddr *)&servaddr,addr_size);
	nBytes=recvfrom(cliSocket,buffer,255,0,NULL,NULL);
	cout<<buffer;
	return 0;
}

