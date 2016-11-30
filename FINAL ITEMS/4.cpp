#include <iostream>

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <sys/msg.h>

using namespace std;

struct msg
{
	long mtype;
	char mbuff[120];
};

void server()
{
	int msgclient, msgserver;

	int msgflag = IPC_CREAT|0666;

	key_t s = 6712;
	key_t r = 6713;

	msgserver = msgget(s, msgflag);
	msgclient = msgget(r, msgflag);

	struct msg ms, mr;

	ms.mtype = 1;

	while (1)
	{
		cin.getline(ms.mbuff, '\n');
		msgsnd(msgserver, &ms, sizeof(ms.mbuff), 0);
		msgrcv(msgclient, &mr, sizeof(mr.mbuff), 1, 0);
		cout << "\n --> " << mr.mbuff << endl;
	}

}

void client()
{
	int msgclient, msgserver;

	int msgflag = IPC_CREAT|0666;

	key_t s = 6712;
	key_t r = 6713;

	msgserver = msgget(s, msgflag);
	msgclient = msgget(r, msgflag);

	struct msg ms, mr;

	ms.mtype = 1;

	while (1)
	{
		
		msgrcv(msgserver, &mr, sizeof(mr.mbuff), 1, 0);
		cout << "\n --> " << mr.mbuff << endl;
		cin.getline(ms.mbuff, '\n');
		msgsnd(msgclient, &ms, sizeof(ms.mbuff), 0);
	}
}

int main(int argc, char const *argv[])
{
	if (argc < 2)
		return 0;

	if (*argv[1]=='s')
		server();

	if (*argv[1]=='c')
		client();

	return 0;
}
