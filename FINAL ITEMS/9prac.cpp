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

	struct sockaddr_storage cliadd;
	socklen_t clilen = sizeof(cliadd);
	int nBytes = recvfrom(sockfd, &m, sizeof(m), 0, (struct sockaddr *) &cliadd, &clilen);

	cout << nBytes;

	cout << m.from << endl << m.to << endl << m.subj << endl << m.content;

	

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

	// connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));

	struct mail m;
	cin.getline(m.from, 30, '\n');
	cin.getline(m.to, 30, '\n');
	cin.getline(m.subj, 30, '\n');
	cin.getline(m.content, 30, '\n');

	socklen_t serv_len = sizeof(serv_addr);
	sendto(sockfd, &m, sizeof(m), 0, (struct sockaddr *) &serv_addr, serv_len);

	close(sockfd);

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