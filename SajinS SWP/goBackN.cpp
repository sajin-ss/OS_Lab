#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <string.h>

#include <fcntl.h>


#define WINDOWSIZE 8

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

    fcntl(newsockfd, F_SETFL, O_NONBLOCK);

	int i = 0;

	int ack_pending = 0;
	int next_ack_no;

	packet p;
	p.info = 0;
	p.seq = 0;
	
	cout << "\n Info : " << i << " Seq : " << p.seq;
	write(newsockfd, &p, sizeof(packet));
	ack_pending++;
	next_ack_no = p.seq;

	packet ack;
	while (1)
	{

		if (i==WINDOWSIZE-1)
			i=-1;

		sleep(3);


		size_t t = recv(newsockfd, &ack, sizeof(ack), 0);

		while (ack_pending >= WINDOWSIZE)	//TIMEOUT DETECTION
		{

//SPEED CONDITION
			/*
			cout << "\n--------------waiting for ack of seq >> " << next_ack_no << endl;
			do
			{
				if (t == sizeof(ack) && ack.seq == next_ack_no)
					break;
				t = recv(newsockfd, &ack, sizeof(ack), 0);

			}while (1);//(t != sizeof(ack) && ack.seq == next_ack_no);

			cout << "\n ----Ack received for Seq : " << ack.seq << "(" << ack.info << ")";
			next_ack_no = (ack.seq+1)%WINDOWSIZE;
			ack_pending--;
			*/


//GO BACK N
		cout  << "\n Acknowledgment " << next_ack_no << " TIMEOUT ..... Go Back N ! ";
			ack_pending = 0;
			i = next_ack_no - 1;
			p.info -= WINDOWSIZE;
		}

		if (t == sizeof(ack))	//ACKNOWL PACKET
		{
			if (ack.seq == next_ack_no)
			{
				cout << "\n ----Ack received for Seq : " << ack.seq << "(" << ack.info << ")";
				next_ack_no = (ack.seq+1)%WINDOWSIZE;
				ack_pending--;
			}
		}
		else
			cout << "\n ----None";

		p.seq = ++i;
		p.info++;
		cout << "\n Info : " << p.info << " Seq : " << p.seq;

		write(newsockfd, &p, sizeof(packet));
		ack_pending++;
	}
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

	fcntl(sockfd, F_SETFL, O_NONBLOCK);

	packet p;
	packet ack;

	int next_seq = 0;

	int err_flag = 0;
	int once = 1;

	while (1)
	{

		sleep(1);
		size_t t = recv(sockfd, &p, sizeof(p), 0);


		if (t==sizeof(p))
		{
			if (once && p.seq == 4)
			{
				cout << endl << " Seq 4 : Error, No Ack Sent";
				err_flag = 1;
				next_seq = 4;
				once = 0;

			}
			else if (err_flag)
			{
				if (p.seq == 4)
				{
					err_flag = 0;
					cout << "\n RECEIVED -> Info : " << p.info << " Seq : " << p.seq << ". Ack sent" ;
					ack.seq = p.seq;
					ack.info = p.info;
	
					sleep(3);
					write(sockfd, &ack, sizeof(ack));
				}
				else
				{
					cout << "\n RECEIVED -> Info : " << p.info << " Seq : " << p.seq << ". DISCARDED" ;
				}
			}
			else
			{
				cout << "\n RECEIVED -> Info : " << p.info << " Seq : " << p.seq << ". Ack sent" ;
				ack.seq = p.seq;
				ack.info = p.info;
	
				sleep(3);
				write(sockfd, &ack, sizeof(ack));
			}
		}
		else
		{
			cout << "\nNone";
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
 ----None
 Info : 1 Seq : 1
 ----Ack received for Seq : 0(0)
 Info : 2 Seq : 2
 ----Ack received for Seq : 1(1)
 Info : 3 Seq : 3
 ----Ack received for Seq : 2(2)
 Info : 4 Seq : 4
 ----None
 Info : 5 Seq : 5
 ----Ack received for Seq : 3(3)
 Info : 6 Seq : 6
 ----None
 Info : 7 Seq : 7
 ----None
 Info : 8 Seq : 0
 ----None
 Info : 9 Seq : 1
 ----None
 Info : 10 Seq : 2
 ----None
 Info : 11 Seq : 3
 Acknowledgment 4 TIMEOUT ..... Go Back N ! 
 ----None
 Info : 4 Seq : 4
 ----None
 Info : 5 Seq : 5
 ----Ack received for Seq : 4(4)
 Info : 6 Seq : 6
 ----Ack received for Seq : 5(5)
 Info : 7 Seq : 7
 ----Ack received for Seq : 6(6)
 Info : 8 Seq : 0
 ----None
 Info : 9 Seq : 1
 ----Ack received for Seq : 7(7)
 Info : 10 Seq : 2
 ----Ack received for Seq : 0(8)
 Info : 11 Seq : 3
 ----Ack received for Seq : 1(9)
 Info : 12 Seq : 4
 ----None
 Info : 13 Seq : 5
 ----Ack received for Seq : 2(10)
 Info : 14 Seq : 6
 ----Ack received for Seq : 3(11)
 Info : 15 Seq : 7
 ----Ack received for Seq : 4(12)
 Info : 16 Seq : 0
 ----None



//RECEIVER

 RECEIVED -> Info : 0 Seq : 0. Ack sent
 RECEIVED -> Info : 1 Seq : 1. Ack sent
 RECEIVED -> Info : 2 Seq : 2. Ack sent
 RECEIVED -> Info : 3 Seq : 3. Ack sent
 Seq 4 : Error, No Ack Sent
 RECEIVED -> Info : 5 Seq : 5. DISCARDED
 RECEIVED -> Info : 6 Seq : 6. DISCARDED
None
 RECEIVED -> Info : 7 Seq : 7. DISCARDED
None
None
 RECEIVED -> Info : 8 Seq : 0. DISCARDED
None
None
 RECEIVED -> Info : 9 Seq : 1. DISCARDED
None
None
 RECEIVED -> Info : 10 Seq : 2. DISCARDED
None
None
 RECEIVED -> Info : 11 Seq : 3. DISCARDED
None
None
 RECEIVED -> Info : 4 Seq : 4. Ack sent
 RECEIVED -> Info : 5 Seq : 5. Ack sent
 RECEIVED -> Info : 6 Seq : 6. Ack sent
 RECEIVED -> Info : 7 Seq : 7. Ack sent
 RECEIVED -> Info : 8 Seq : 0. Ack sent
 RECEIVED -> Info : 9 Seq : 1. Ack sent
 RECEIVED -> Info : 10 Seq : 2. Ack sent
 RECEIVED -> Info : 11 Seq : 3. Ack sent
 RECEIVED -> Info : 12 Seq : 4. Ack sent
^C


*/
