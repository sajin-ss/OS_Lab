#include <iostream>

#include <sys/statvfs.h>

using namespace std;


int main(int argc, char const *argv[])
{
	
	struct statvfs Data;

	char path[128];

	cout << "\n Enter path : " ;
	cin >> path;

	statvfs(path, &Data);

	cout << Data.f_bsize;
	cout << Data.f_frsize;
	cout << Data.f_blocks;
	return 0;
}