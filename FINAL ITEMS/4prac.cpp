#include <iostream>

#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>


#include <sys/msg.h>
using namespace std;


struct msg
{
	long mtype;
	char mbuff[120];
};

int main(int argc, const char *argv[])
{
	if (argc < 2)
		return 0;

	int msgserver, msgclient;

	key_t s = 4781;
	key_t r = 4782;


	int flag = IPC_CREAT | 0666;
	msgserver = msgget(s, flag);
	msgclient = msgget(r, flag);


	struct msg ms;
	struct msg mr;

	if (*argv[1] == 's')
	{
		while(1)
		{
			cin.getline(ms.mbuff, 100);
			msgsnd(msgserver, &ms, sizeof(ms.mbuff), 0);
			msgrcv(msgclient, &mr, sizeof(mr.mbuff), 1, 0);
			cout << " --> " << mr.mbuff << endl;
		}
	}
	else
	{
		while(1)
		{
			msgrcv(msgserver, &mr, sizeof(mr.mbuff), 1, 0);
			cout << " --> " << mr.mbuff << endl;
			cin.getline(ms.mbuff, 100);
			msgsnd(msgclient, &ms, sizeof(ms.mbuff), 0);
		}
	}
	return 0;
}