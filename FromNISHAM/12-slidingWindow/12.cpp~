#include<iostream>
#include<stdio.h>
#include<sys/types.h>
#include<string.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<fstream>
#define sa struct sockaddr
#define listenq 5
#define defport "132445"
using namespace std;
class receiver
{
public:
  char* filename;
  char* port;
  receiver(char* filename, char* port)
  {
    this->filename = filename;
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
    char buffer[100];
    bzero(buffer,sizeof(buffer));
    ofstream f(filename);
    buffer[0]='r';
    while(buffer[0]!='\0')
      {
	read(connfd,buffer,100);
	f<<buffer;
	f<<'\n';
      }
    f.close();
    printf("the file received");
  }
};

class sender
{
public:
  char *filename;
  char *port;
  sender(char *filename, char *port)
  {
    this->filename = filename;
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

    char buffer[100];
    ifstream f(filename);
    while(f.getline(buffer,sizeof(buffer)))
      write(sockfd,buffer,100);
    char end='\0';
    memcpy(buffer,&end,1);
    write(sockfd,buffer,100);
    f.close();
    cout<<"sent"<<endl;
  }
};

int main(int argc, char **argv)
{
  if(argc<3)
    {
      cout<<"usage : netcat s/c filename [port]"<<endl;
    }
  //cout<<argv[1];
  if (argv[1][0]=='s')
    {
      //      cout<<"sender";
      char port[10] = defport;
      if(argc>3)
	strcpy(port,argv[3]);
      //      cout<<port;
      sender s(argv[2],port);
      s.send();
    }
  else
    {
      //      cout<<"receiver";
      char port[10] = defport;
      if(argc>3)
	strcpy(port,argv[3]);
      //      cout<<port;
      receiver r(argv[2],port);
      r.receive();
    }
  return 0;
}
