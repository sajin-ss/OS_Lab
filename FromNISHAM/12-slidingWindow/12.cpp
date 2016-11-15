#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <fstream>

#define max 20
#define sa struct sockaddr
#define listenq 5
#define defport "132445"
using namespace std;

class receiver
{
public:
  int a;
  int ack = 0;
  int size;
  char* port;
  receiver(char* port)
  {
    this->port = port;
  }
  void receive()
  {
    int fd,sockfd,listenfd,connfd;
    pid_t childpid;
    socklen_t client;
    
    struct sockaddr_in servaddr, cliaddr;
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    servaddr.sin_port = htons(atoi(this->port));
    bind(listenfd,(sa*)&servaddr,sizeof(servaddr));
    listen(listenfd,listenq);
    client=sizeof(cliaddr);
    
    connfd=accept(listenfd,(sa*)&cliaddr,&client);
    fcntl(connfd, F_SETFL, O_NONBLOCK);
    char buffer[100];
    bzero(buffer,sizeof(buffer));

    read(connfd, &size, sizeof(int));
    
    //cout<<"Size is : "<<size<<endl;
    int i=0;
    
    while(i<size)
      {
	read(connfd, &a, sizeof(int));
	cout<<"Recieved : "<<a<<endl;
	if(rand()%2)
	  {
	    cout<<"Sent Acknowledge Number : "<<ack<<endl;
	    write(connfd, &ack, sizeof(int));
	    sleep(7);
	    ack++;
	    i++;
	  }
	else
	  {
	    int random = rand()%1000;
	    cout<<"no acknowledge sent"<<endl;
	    sleep(7);
	    //write(connfd, &random, sizeof(int));
	  }
      }
    
    
    /*ofstream f(filename);

      buffer[0]='r';
      while(buffer[0]!='\0')
      {
      read(connfd,buffer,100);
      f<<buffer;
      f<<'\n';
      }
      f.close();
    */
    printf("the file received");
  }
};

class sender
{
public:
  int *a;
  int tries;
  int size;
  int retryAfter;
  char *port;
  sender(int arr[],int size, char *port)
  {
    tries = 5;
    retryAfter=1000000;
    this->a = arr;
    this->size=size;
    this->port = port;
    
  }
  
  void send()
  {
    int sockfd;
    char fname[25];
    int len;

    struct sockaddr_in servaddr,cliaddr;
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    servaddr.sin_port=htons(atoi(this->port));
    inet_pton(AF_INET,this->port,&servaddr.sin_addr);
    connect(sockfd,(sa*)&servaddr,sizeof(servaddr));
    
    fcntl(sockfd, F_SETFL, O_NONBLOCK);
    write(sockfd, &size, sizeof(int));
    int i=0;
    int tried=0;
    int after=0;
    while(i<size)
      {
	write(sockfd, &a[i], sizeof(int));
	int ack;
	cout<<"Sent : "<<a[i]<<endl;
	sleep(7);
	size_t n=recv(sockfd, &ack, sizeof(int),0);
	if(n!=4)
	  {
	    cout<<"No Acknowledge for : "<<i<<", Retrying"<<endl;
	    sleep(7);
	    tried++;
	    if(tried>5)
	      {
		cout<<"No response after "<<tried<<" retries , ending transmission\n";
		break;
	      }
	    //break;
	  }
	else if (ack==i)
	  {
	    tried = 0;
	    cout<<"Received Acknowledge Number : "<<ack<<endl;
	    i++;
	  }
      }
    /*
      char buffer[100];
      ifstream f(filename);
      while(f.getline(buffer,sizeof(buffer)))
      write(sockfd,buffer,100);
      char end='\0';
      memcpy(buffer,&end,1);
      write(sockfd,buffer,100);
      f.close();
      cout<<"sent"<<endl;
    */
  }
};

int main(int argc, char **argv)
{
  if(argc<2)
    {
      cout<<"usage : ./12 s/c [port]"<<endl;
    }
  //cout<<argv[1];
  if (argv[1][0]=='s')
    {
      //      cout<<"sender";
      char port[10] = defport;
      if(argc>3)
	strcpy(port,argv[3]);
      //      cout<<port;
      int a[] = {10,9,8,7,6,5,4,3,2,1};
      sender s(a,10,port);
      s.send();
    }
  else
    {
      //      cout<<"receiver";
      char port[10] = defport;
      if(argc>3)
	strcpy(port,argv[3]);
      //      cout<<port;
      receiver r(port);
      r.receive();
    }
  return 0;
}
