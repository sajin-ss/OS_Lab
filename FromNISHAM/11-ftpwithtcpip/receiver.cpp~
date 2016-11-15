#include<iostream>
#include<stdio.h>
#include<sys/types.h>
#include<string.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#define sa struct sockaddr
#define listenq 5
int main()
{
  int fd,sockfd,listenfd,connfd;
  pid_t childpid;
  socklen_t client;
  struct sockaddr_in servaddr, cliaddr;
  listenfd = socket(AF_INET, SOCK_STREAM, 0);
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
  char a[10] = "15236";
  servaddr.sin_port = htons(atoi(a));
  bind(listenfd,(sa*)&servaddr,sizeof(servaddr));
  listen(listenfd,listenq);
  client=sizeof(cliaddr);
  connfd=accept(listenfd,(sa*)&cliaddr,&client);
  char buffer[100];
  FILE* fp;
  read(connfd,buffer,100);
  fp=fopen("myfilereceived.txt","w");
  fprintf(fp,"%s",buffer);
  printf("the file received");
}
