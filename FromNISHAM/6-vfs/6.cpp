#include<stdio.h>
#include<iostream>
#include<unistd.h>
#include<sys/statvfs.h>
using namespace std;

int main()
{
  struct statvfs data;
  char path[128]="/home/us/s13/s1338/s7/6-vfs";
  int ret=statvfs(path,&data);
  if(ret==-1)
    return -1;
  cout<<"free blocks : "<<data.f_blocks<<endl;
  cout<<"b free : "<<data.f_bfree<<endl;
  cout<<"free size : "<<data.f_frsize<<endl;
  return 0;
}
