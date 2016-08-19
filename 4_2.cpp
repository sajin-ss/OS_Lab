/* Pgm. 4
   SAJIN - 052 - s7
   MSGQUEUE CHAT CLIENT
   21/07/2016
*/

#include <iostream>
#include <unistd.h>
#include <string>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

struct msg
{
   long mtype;
   char mbuff[120];
};


int main()
{

   int msgserver, msgclient;
   int msgflag = IPC_CREAT|0666;
   key_t s = 6781;
   key_t r = 6782;
   
   msgserver = msgget(s, msgflag);
   msgclient = msgget(r, msgflag);
   
   msg ms, mr;
   ms.mtype = 1;
   
   while (1)
   {
    
     msgrcv(msgserver, &mr, sizeof(mr.mbuff), 1, 0);
     cout << "-> " << mr.mbuff << "\n";

     cin.getline(ms.mbuff, 120);
     msgsnd(msgclient, &ms, sizeof(ms.mbuff), 0);
    
   }
   
   
 return 0;
}


/*
OUTPUT
-----

-> Hi
Domo
-> Katon
Gokekyou
-> no Jutsu
Amaterasu
-> Tsukyomi
Bye
-> ok Bye
^C

*/
