#include <iostream>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>

#include <map>

#define WINDOWSIZE 8

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

	fcntl(newsockfd, F_SETFL, O_NONBLOCK);

	struct packet p, ack;

	map<int, int> buff;

	int i = 0;
	int z;
	int seq = 0;

	int gobackN = 0;
	int next_ack_exp;

	p.seq = seq;
	p.info = i;


	cout << "\n Info : " << p.info << " Seq : " << p.seq;
	send(newsockfd, &p, sizeof(packet), 0);
	next_ack_exp = seq;


	while (1)
	{
		sleep(3);
		size_t t = recv(newsockfd, &ack, sizeof(ack), 0);


		if ((seq+1)%WINDOWSIZE == next_ack_exp)
		{
			cout << "\nTIMEOUT for seq # " << next_ack_exp << "! Go Back N .. ";
			gobackN = 1;
			seq = next_ack_exp-1;
			z = 0;
		}

		if (t==sizeof(ack))
		{
			if (ack.info == next_ack_exp)
			{
				cout << endl << " Ack got for " << buff[ack.info] << "("<<ack.info<<")";
				next_ack_exp = (next_ack_exp+1)%WINDOWSIZE;
			}

		}

		if (seq+1 == WINDOWSIZE)
			seq = -1;

	

		if (z < WINDOWSIZE && gobackN)
		{
			z++;
			p.seq = ++seq;
			p.info = buff[p.seq];
		}
		else
		{
			gobackN = 0;
			p.seq = ++seq;
			p.info = ++i;
		}

		buff[p.seq] = p.info;

		cout << "\n Info : " << p.info << ". Seq : " << p.seq;
		send(newsockfd, &p, sizeof(packet), 0);
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

	fcntl(sockfd, F_SETFL, O_NONBLOCK);

	struct packet p, ack;
	int next_exp = 0;

	int once = 1;

	while(1)
	{

		sleep(1);
		size_t t = recv(sockfd, &p, sizeof(p), 0);

		if (t == sizeof(p))
		{
			if (p.seq == next_exp)
			{
				if (once && p.seq==4)
				{
					once = 0;
					cout << "\n Seq : " << p.seq << " Error. No ack sent ";
				}
				else
				{
					cout << "\n Info got: " << p.info << ". Seq : " << p.seq;
					next_exp = (next_exp+1)%WINDOWSIZE;

					ack.info = p.seq;
					cout << "\n Ack for Info " << p.info << " Seq : " << p.seq << "sent";

					sleep(3);
					send(sockfd, &ack, sizeof(ack), 0);
				}
			}
			else
			{
				cout << "\n Seq : " << p.seq << " Discarded. ";
			}
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