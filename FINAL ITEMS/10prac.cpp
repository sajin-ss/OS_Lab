#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#include <sys/socket.h>
#include <netinet/in.h>


using namespace std;

void sender(int port)
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

while(1)
{
	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);


	int pid = fork();

	if (pid)
	{
		wait(NULL);
		close(newsockfd);
	}
	else
	{
		fstream fin("input2", ios::in);
		char buff[100];
		while (!fin.eof())
		{
			fin.getline(buff, 100);
			send(newsockfd, &buff, sizeof(buff), 0);
		}
		buff[0] = '\0';
		buff[1] = '\0';
		write(newsockfd, &buff, sizeof(buff));
	
			fin.close();
		cout << "\nFile sent\n";

	}

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

	connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));

	fstream fout("gotFile.txt", ios::out);

	char buff[100];
	while (1)
	{
		read(sockfd, &buff, sizeof(buff));
		if (buff[0]=='\0' && buff[1]=='\0')
			break;
		fout << buff << endl;
	}


	cout << "\nFile received\n";
	fout.close();
	close(sockfd);

}

int main(int argc, char const *argv[])
{
	if (argc < 3)
		return 0;

	if (*argv[1]=='s')
		sender(atoi(argv[2]));
	else
		client(atoi(argv[2]));

	return 0;
}