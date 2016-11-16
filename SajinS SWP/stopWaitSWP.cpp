#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <string.h>


using namespace std;


struct packet{
	int info;
	int seq;
};

void sender(const char *port)
{
	int sockfd, newsockfd;
	struct sockaddr_in serv_addr, cli_addr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&serv_addr, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(port));

	bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
	listen(sockfd, 2);


	socklen_t client = sizeof(cli_addr);
	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &client);

	int i = 0;
	int next_seq = 0;

	packet p;
	p.info = i;
	p.seq = 0;
	next_seq = 1;
	
	cout << "\n Info : " << i << " Seq : " << p.seq;
	write(newsockfd, &p, sizeof(packet));

	packet ack;
	while (1)
	{
		sleep(3);
		read(newsockfd, &ack, sizeof(packet));
		if (ack.info == p.seq)
		{
			//Next Info

			p.info = ++i;
			p.seq = next_seq;
			next_seq = !next_seq;

			cout << "\nAck got. \n Info : " << i << " Seq : " << p.seq;

if (i==7)
	break;

			write(newsockfd, &p, sizeof(packet));
			// sleep(3);
		}
		else
		{
			//Re Send
			cout << "\n Ack for " << i << " not got . Retrying. ";
			cout << "\n Info : " << i << " Seq : " << p.seq;
			write(newsockfd, &p, sizeof(packet));
			// sleep(3);
		}
	}

	p.seq = 3;
	write(newsockfd, &p, sizeof(packet));
	// sleep(3);

}

void receiver(const char *port)
{
	int sockfd;
	struct sockaddr_in serv_addr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(port));
 

    // inet_pton(AF_INET, port, &serv_addr);
	connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));

	packet p;
	packet ack;

	int next_seq = 0;

	while (1)
	{
		// sleep(3);

		read(sockfd, &p, sizeof(p));
		if (p.seq == 3)
			break;

		if (p.seq == next_seq)
		{
			next_seq = !next_seq;
			if (rand()%2==0)
			{
				cout <<  "\n Info " << p.info << " received. Ack sent.";
				ack.info = p.seq;
				ack.seq = 2;
				write(sockfd, &ack, sizeof(ack));
			}
			else
			{	//Garbled
				cout <<  "\n Info " << p.info << " received. Ack sent.";
				ack.info = p.seq*3;
				ack.seq = 2;
				write(sockfd, &ack, sizeof(ack));
			}
		}
		else
		{
			//ack Resend
				cout <<  "\nAlready received. Ack RESEND.";
				ack.info = p.seq;
				ack.seq = 2;
				write(sockfd, &ack, sizeof(ack));

		}



		
		
	}

}

int main(int argc, char const *argv[])
{


	if (argc < 3)
		return 0;

	char c = *argv[1];

	if (c=='s')
	{
		//SENDER SENDER SENDER
		sender(argv[2]);
	}
	else
	{
		//RECEIVER RECEIVER RECEIVER
		receiver(argv[2]);
	}
	

cout << "\n DISCONNECTED ";

	return 0;
}


/* OUTPUT

//SENDER
 Info : 0 Seq : 0
Ack got. 
 Info : 1 Seq : 1
Ack got. 
 Info : 2 Seq : 0
Ack got. 
 Info : 3 Seq : 1
 Ack for 3 not got . Retrying. 
 Info : 3 Seq : 1
Ack got. 
 Info : 4 Seq : 0
Ack got. 
 Info : 5 Seq : 1
 Ack for 5 not got . Retrying. 
 Info : 5 Seq : 1
Ack got. 
 Info : 6 Seq : 0
Ack got. 
 Info : 7 Seq : 1
 DISCONNECTED 



 //RECEIVER
  Info 0 received. Ack sent.
 Info 1 received. Ack sent.
 Info 2 received. Ack sent.
 Info 3 received. Ack sent.
Already received. Ack RESEND.
 Info 4 received. Ack sent.
 Info 5 received. Ack sent.
Already received. Ack RESEND.
 Info 6 received. Ack sent.


 */