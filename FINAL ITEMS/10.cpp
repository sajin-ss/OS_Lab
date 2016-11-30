#include <iostream>
#include <stdlib.h>

#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <string.h>
#include <fstream>


using namespace std;


void server(int port)
{
	int sockfd, newsockfd;
	struct sockaddr_in serv_addr, cli_addr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(port);

	bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));

	listen(sockfd, 2);

	socklen_t clilen = sizeof(cli_addr);
	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen );

	char buff[128];
	fstream fout("out.txt", ios::out);
	while(1)
	{
		read(newsockfd, buff, sizeof(buff));
		// cout << buff;
		if (buff[0]=='\0' && buff[1]=='o')
			break;
		// cout << buff;

		fout << buff << endl;
	}

	cout << "\n FilE Received\n";

	fout.close();

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

	connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));


	char buff[128];
	fstream fin("1.cpp");
	while(!fin.eof())
	{

		fin.getline(buff, 100);
		// cout << buff;
		write(sockfd, buff, sizeof(buff));
	}
	buff[0] = '\0';
	buff[1] = 'o';
	write(sockfd, buff, sizeof(buff));

	fin.close();

	cout << " \n FILE SENT \n";
}

int main(int argc, char const *argv[])
{

	if (argc < 3)
		return 0;

	if (*argv[1]=='s')
		server(atoi(argv[2]));
	else if (*argv[1]=='c')
		client(atoi(argv[2]));
	
	return 0;
}
