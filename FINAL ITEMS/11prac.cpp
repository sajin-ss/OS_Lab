#include <iostream>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;

struct packet
{
	int seq;
	int info;
};

void sender(int port)
{
	int sockfd, newsockfd;
	struct sockaddr_in serv_addr, cli_addr;

	sockfd = socket(PF_INET, SOCK_STREAM, 0);

	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(port);

	bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));

	listen(sockfd, 2);

	socklen_t clilen = sizeof(cli_addr);
	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

	packet p, ack;

	int i = 0;
	int next_seq = 0;

	p.info = i++;
	p.seq = next_seq;
	next_seq != next_seq;

	write(newsockfd, &p, sizeof(p));
	cout << "\n Sent : Info : "<<p.info << " Seq : "<< p.seq;

	while(1)
	{
		read(newsockfd, &ack, sizeof(ack));

		if (ack.info != p.seq)
		{
			write(newsockfd, &p, sizeof(p));
		}
		else
		{
			p.info = i++;
			p.seq = next_seq;
			next_seq != next_seq;
			write(newsockfd, &p, sizeof(p));
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

	packet p, ack;

	int nex_seq_exp = 0;

	while(1)
	{
		read(sockfd, &p, sizeof(p));

		if (p.seq == nex_seq_exp)
		{
			cout << "Got : Info : " << p.info << " Seq : " << p.seq << endl << "Ack sent\n";

			ack.info = p.seq;
			ack.seq = 0;
			nex_seq_exp != nex_seq_exp;
			write(sockfd, &ack, sizeof(ack));
		}
		else
		{
			cout << "Seq already got. Ack resend \n";
			ack.info = p.seq;
			ack.seq = 0;
			write(sockfd, &ack, sizeof(ack));
		}

	}
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