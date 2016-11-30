#include <iostream>

#include <sys/statvfs.h>

using namespace std;

int main(int argc, char const *argv[])
{
	char path[100];
	struct statvfs Data;

	cout << "Path : ";
	cin >> path;

	statvfs(path, &Data);

	cout << Data.f_bsize << endl;
	cout << Data.f_frsize << endl;
	cout << Data.f_blocks << endl;
	cout << Data.f_bfree << endl;

	return 0;
}