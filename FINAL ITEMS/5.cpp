#include <iostream>
#include <stdlib.h>

#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#include <sys/socket.h>
#include <netinet/in.h> //****************
#include <string.h>

using namespace std;

void server(int port)
{
	int sockfd, newsockfd;

	struct sockaddr_in serv_addr, cli_addr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

// if (sockfd < 0)
// 	return;

	bzero(&serv_addr, sizeof(serv_addr));	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(port);

	bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));

	listen(sockfd, 2);

	socklen_t clilen = sizeof(cli_addr);		//*****************************************
	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);


	while(1)
	{
		char a[120];
		cin.getline(a, '\n');
		write(newsockfd, a, sizeof(a));
		read(newsockfd, a, sizeof(a));
		cout << "\n --> " << a << endl;
	}

}

void client(int port)
{
	int sockfd;

	struct sockaddr_in serv_addr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(port);

	// bind(sockfd, (struct sockaddr *) serv_addr, sizeof(serv_addr));
	connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));

	while(1)
	{
		char a[120];
		read(sockfd, a, sizeof(a));
		cout << "\n --> " << a << endl;
		cin.getline(a, '\n');
		write(sockfd, a, sizeof(a));
	}
}

int main(int argc, char const *argv[])
{
	if (argc < 3)
		return 0;

	if (*argv[1] == 's')
		server(atoi(argv[2]));
	else if (*argv[1] == 'c')
		client(atoi(argv[2]));

	return 0;
}