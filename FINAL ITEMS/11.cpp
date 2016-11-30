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

	struct packet p, ack;

	int i = 0;
	int seq = 0;
	int next_seq = !seq;

	p.seq = seq;
	p.info = i;

	cout << "\n Info : " << p.info << " Seq : " << p.seq;
	send(newsockfd, &p, sizeof(packet), 0);

	while (1)
	{
		sleep(1);
		recv(newsockfd, &ack, sizeof(ack), 0);
		if (ack.info == seq)
		{
			cout << endl << " Ack for seq " << seq << " got .";
			seq = next_seq;
			next_seq = !seq;

			p.seq = seq;
			p.info = ++i;

			cout << endl << " Info : " << p.info << " Seq : " << p.seq;
			send(newsockfd, &p, sizeof(p), 0);

		}
		else
		{
			cout << endl << " Ack for seq " << seq << " Not got . RESENDING...";
			send(newsockfd, &p, sizeof(packet), 0);
		}

		if (i==7)
			break;
	}
	p.seq = 2;
	send(newsockfd, &p, sizeof(p), 0);
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

	struct packet p, ack;
	int next_exp = 0;
	while(1)
	{
		recv(sockfd, &p, sizeof(p), 0);

		if (p.seq == 2)
			break;

		if (rand()%2==1)
			ack.info = p.seq*3;
		else
			ack.info = p.seq;

		if (p.seq == next_exp)
		{
			cout << endl << " Info " << p.info << " got. Ack sent ";
			next_exp = !next_exp;
			send(sockfd, &ack, sizeof(ack), 0);
		}
		else
		{
			cout << endl << " Info " << p.info << " already got. Ack RESEND ";
			send(sockfd, &ack, sizeof(ack), 0);
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