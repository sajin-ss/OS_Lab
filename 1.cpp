/*
   Pgm 1
   04/07/2016
   SAJIN-051-S7
   FORKING
*/

#include<sys/types.h>
#include<unistd.h>
#include<iostream>
using namespace std;

int main(int argc, char const *argv[])
{
	int pid, ppid, cpid;
	cout<<"main process with id" << getpid()<<endl;
	cout<<"splitting"<<endl;
	pid = fork();
	cout<<"forked id"<< pid<<endl;

	if(pid == 0)
	{
		cout<<"process 2 with pid : " <<getpid()<<endl;
		cout<<"process 2 with ppid : " <<getppid()<<endl;
		cout<<"process 2 with upid : " <<getuid()<<endl;
		cout<<"process 2 with egid : " <<getegid()<<endl;
	}
	else{
		cout<<endl;
		cout<<"process 1 with pid : " <<getpid()<<endl;
		cout<<"process 1 with ppid : " <<getppid()<<endl;
		cout<<"process 1 with upid : " <<getuid()<<endl;
		cout<<"process 1 with egid : " <<getegid()<<endl;
		int pid1 = fork();
		if(pid1 == 0){
			cout<<endl;
			cout<<"process 4 with pid : " <<getpid()<<endl;
			cout<<"process 4 with ppid : " <<getppid()<<endl;
			cout<<"process 4 with upid : " <<getuid()<<endl;
			cout<<"process 4 with egid : " <<getegid()<<endl;
			int pid2 = fork();
			if(pid2 == 0){
			cout<<endl;
			cout<<"process 3 with pid : " <<getpid()<<endl;
			cout<<"process 3 with ppid : " <<getppid()<<endl;
			cout<<"process 3 with upid : " <<getuid()<<endl;
			cout<<"process 3 with egid : " <<getegid()<<endl;
			}
		}
	}
	return 0;
}


/*
OUTPUT
------

main process with id31003
splitting
forked id31004

process 1 with pid : 31003
process 1 with ppid : 23318
process 1 with upid : 1052
process 1 with egid : 1052
forked id0
process 2 with pid : 31004
process 2 with ppid : 31003
process 2 with upid : 1052
process 2 with egid : 1052

process 4 with pid : 31005
process 4 with ppid : 31003
process 4 with upid : 1052
process 4 with egid : 1052

process 3 with pid : 31006
process 3 with ppid : 1
process 3 with upid : 1052
process 3 with egid : 1052


*/
