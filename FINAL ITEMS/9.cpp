#include <iostream>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;


struct mail{
	char from[128];
	char to[128];
	char subj[128];
	char content[1024];
};

void sender(int port)
{
	int sockfd;
	struct sockaddr_in serv_addr;

	sockfd = socket(PF_INET, SOCK_DGRAM, 0);

	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(port);

	bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));


	struct mail m;
	sockaddr_storage servsto;
	socklen_t stolen = sizeof(servsto);


	/* Try to receive any incoming UDP datagram. Address and port of 
      requesting client will be stored on serverStorage variable */
	int nBytes = recvfrom(sockfd, &m, sizeof(m), 0, (struct sockaddr *) &servsto, &stolen);

	cout << "\n FROM : " << m.from; 
	cout << "\n TO : " << m.to; 
	cout << "\n SUBJECT : " << m.subj; 
	cout << "\n CONTENT : " << m.content; 

	char buff[128];
	strcpy(buff, "received");
	sendto(sockfd, buff, sizeof(buff), 0, (struct sockaddr *) &servsto, stolen);
}

void client(int port)
{
	int sockfd;
	struct sockaddr_in serv_addr;

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(port);


	connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));

struct mail m;
	cin.getline(m.from, 128, '\n');
	cin.getline(m.to, 128, '\n');
	cin.getline(m.subj, 128, '\n');
	cin.getline(m.content, 1024, '\n');

	socklen_t servlen = sizeof(serv_addr);
	sendto(sockfd, &m, sizeof(m), 0, (struct sockaddr *) &serv_addr, servlen);

	char buff[128];
	int nBytes = recvfrom(sockfd, buff, sizeof(buff), 0, NULL, NULL);

	cout << "\n " << buff;
}

int main(int argc, char const *argv[])
{
	
	if (argc < 3)
		return 0;

	if (*argv[1] == 's')
		sender(atoi(argv[2]));
	else if (*argv[1] == 'c')
		client(atoi(argv[2]));

	return 0;
}