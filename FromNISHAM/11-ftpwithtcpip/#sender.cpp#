#include<iostream>
#include<stdio.h>
#include<sys/types.h>
#include<string.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#define sa struct sockaddr
int main()
{
  int sockfd;
  char fname[25];
  int len;
  struct sockaddr_in servaddr,cliaddr;
  sockfd=socket(AF_INET,SOCK_STREAM,0);
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family=AF_INET;
  servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
  char a[10]="15236";
  servaddr.sin_port=htons(atoi(a));
  inet_pton(AF_INET,a,&servaddr.sin_addr);
  connect(sockfd,(sa*)&servaddr,sizeof(servaddr));
  char buffer[100];
  FILE *f;
  f=fopen("myfile.txt","r");
  fscanf(f,"%s",buffer);
  write(sockfd,buffer,100);
  printf("the file was sent");
}
