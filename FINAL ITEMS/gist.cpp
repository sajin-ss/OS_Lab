#include <iostream>
/*Socket obviously required*/
#include <sys/socket.h>
/*Any shit ending with _t is prolly from this library*/
#include <sys/types.h>
#include <string>
#include "string.h"
#include "unistd.h"
#include "netinet/in.h"
#define defPort 142536
#define defPortDgram 253614
#define max 256
using namespace std;
/*Some random class to send and receive as a class instead of as simple character stream*/
class mail
{
public:
	char *to,*from,*message;
	mail(char* a,char* b,char* c): from(a),to(b),message(c){};
	void print()
	{
		cout<<endl;
		cout<<"from :"<<from<<endl;
		cout<<"to :"<<to<<endl;
		cout<<"message :"<<message<<endl;
		cout<<endl;
	}
};
// To exit out of program on error, should be pretty easy to understand
void exitError(string error)
{
	cout<<error<<endl;
	exit(-1);
}

/*To use as parent class for stuff that uses datagram socket*/
class datagramSocketEndPoint
{
protected:
	/*Dsock is the socket descriptor*/
	int dsock;
	/*Buffer is made void * so that it can even point to random class objects if needed*/
	void *buf;
	sockaddr_storage returnAddress;
public:
	datagramSocketEndPoint()
	{
		/*Initialise a datagramsocket*/
		buf = new char[max];
		dsock = socket(AF_INET,SOCK_DGRAM,0);
		if (dsock<0)
			exitError("cannot create datagramSocket");
	}
	/*Send data*/
	void sendd(void *data,int size,sockaddr *destinationAddress)
	{
		/*man pages are your friend*/
		sendto(dsock,data,size,0,destinationAddress,sizeof(sockaddr_in));
	}
	/*receive a buffer from sockfd*/
	void received(int type)
	{
		socklen_t dummy = sizeof(sockaddr_in);
		switch(type)
		{
			/*Receive normal character stream*/
			case 0:	
			/*Since buffer is void*, cast is used to convert to character*/
			recvfrom(dsock,buf,max,0,(sockaddr *)&returnAddress,&dummy);
			cout<<(char *)buf;
			break;

		}
	}
};
/*Superclass for classes that use streamsockets*/
class streamSocketEndPoint
{
protected:
	/*buffer is used to send data, and sockfd is socket descriptor*/
	int sockfd;
	void *buf;
public:
	/*initialise buffer and socket descriptor*/
	streamSocketEndPoint()
	{
		buf = new char[max];
		/*man socket (or man 2 socket in some systems to know what each are)*/
		sockfd=socket(AF_INET,SOCK_STREAM,0);
		if (sockfd<0)
			exitError("cannot create socket");
	}
	/*Send data*/
	void send(void *data,int size)
	{
		write(sockfd,data,size);
	}
	/*receive a buffer from sockfd*/
	void receive(int type=0)
	{
		switch(type)
		{
			/*Receive normal character stream*/
			case 0:	
			/*Since buffer is void*, cast is used to convert to character*/
			read(sockfd,buf,max);
			cout<<(char *)buf;
			break;

			/*Receive a class object*/
			case 1:
			read(sockfd,buf,sizeof(mail));
			((mail *)buf)->print();
			break;
		}
	}

	~streamSocketEndPoint()
	{
		close(sockfd);
	}

};
/*Sender class is the server
server will bind the address, listen to it, and when accepting a 
connection will create a new client socket so that the listening socket is not interupted
*/
class sender : public streamSocketEndPoint, public datagramSocketEndPoint
{
public:
	/*bind and listen, accept*/
	sockaddr_in mybind(int port,int &socket)
	{
		/*Create a sockaddr_in type, with the same family and given port*/
		struct sockaddr_in saddr;
		saddr.sin_family = AF_INET;
		saddr.sin_port = htons(port);
		saddr.sin_addr.s_addr=htonl(INADDR_ANY);
		/*try and bind the sockfd to it*/
		if(bind(socket,(sockaddr *)&saddr,sizeof(saddr))<0)
			exitError("Cannot bind socket");
		return saddr;
	}
	void bnla(int port)
	{
		sockaddr_in saddr=mybind(port,sockfd);
		/*Listen for incoming connection (only 1 parallel connection given)*/
		listen(sockfd,1);
		/*dummy, not required later*/
		socklen_t dummy = sizeof(sockaddr_in);
		/*overwriting socket for client via accept
		  REALLY REALLY BAD, DONT DO LIKE THIS, MAKE A NEW SOCKET
		  man 2 accept for details
		*/
		  sockfd = accept(sockfd,(sockaddr *)&saddr,&dummy);
		}
		void bnlad(int port)
		{
			sockaddr_in saddr=mybind(port,dsock);
		}
		sockaddr* getClientAddress(int port)
		{
			return (sockaddr *) &areturnAddress; 
		}

	};
/*Receiver class or client class will connect to server and send data*/
	class receiver : public streamSocketEndPoint, public datagramSocketEndPoint
	{
	public:
	/*Try and connect to server port*/
		void con(int port)
		{
		/*sock addr data type, add port and family, connect*/
			sockaddr_in saddr;
			saddr.sin_family = AF_INET;
			saddr.sin_port = htons(port);
			saddr.sin_addr.s_addr=htonl(INADDR_ANY);
			if(connect(sockfd,(sockaddr *)&saddr,(socklen_t)sizeof(saddr))<0)
				exitError("cannot connect");
		}
		sockaddr_in getServerAddress(int port)
		{
		/*sock addr data type, add port and family*/
			sockaddr_in saddr;
			saddr.sin_family = AF_INET;
			saddr.sin_port = htons(port);
			saddr.sin_addr.s_addr=htonl(INADDR_ANY);
			return saddr;
		}
	};
/*driver program for above*/
	int main(int argc, char const *argv[])
	{
		char opt='s';
		if(rand()%5==0)
		{

			cout<<"Server or Client (s/C)?: ";
			cin>>opt;
			if(opt!='s')
			{
		/*Init receiver*/
				receiver r;
		/*Connect to port*/
				r.con(defPort);


		/*Single one send*/
				char a[20]="data from client";
				r.send(a,strlen(a));


		/*Single one receive*/
				r.receive();

		/*Send object instead of char stream*/
				mail *m = new mail("from@from.com","to@to.com","message is message");
				r.send(m,sizeof(mail));
			}
			else
			{
		/*init sender*/
				sender s;
		/*bind, listen accept from port*/
				s.bnla(defPort);


		/*Single one receive*/
				s.receive();


		// Single one send
				char a[20]="data from server";
				s.send(a,strlen(a));

			/*Receive mail (type 1 message)*/
				s.receive(1);
			}
		}
		else
		{

			cout<<"Server or Client (s/C)?: ";
			cin>>opt;
			if(opt!='s')
			{
			/*STREAM SOCKET*/
		/*Init receiver*/
				receiver r;
			/*DATAGRAM SOCKET*/
				sockaddr_in server = r.getServerAddress(defPortDgram);
				char a[20]="data from client";
				r.sendd(a,strlen(a),(sockaddr*) &server);

				r.received(0);
			}
			else
			{
		/*init sender*/
				sender s;
			/*DATAGRAM SOCKET*/
				s.bnlad(defPortDgram);
				s.received(0);

				sockaddr *client = s.getClientAddress(defPortDgram);
				char a[20]="data from server";
				s.sendd(a,strlen(a),client);
			}

		}
		return 0;
	}
