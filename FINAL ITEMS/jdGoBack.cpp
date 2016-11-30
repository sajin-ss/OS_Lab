/*
Go Back-N SW simulation
*/

#include<iostream>
#include<cstring>
#include<random>
#include<map>
#include<climits>

using namespace std;

int window_size = 5;

class Socket{
	map<int,char> data;
public:
	int receive(int seq){
		if(data[seq])
			return seq;
		else
			return INT_MAX;
	}
	void send(char c,int seq){
		if(random()%2){
			data[seq] = c;
		}
	}
	string receivedData(){
		string ret = "";
		for(auto i:data){
			ret+=i.second;
		}
		return ret;
	}
};


int main(){
	char in[100];
	cout<<"Enter input string : ";
	cin.getline(in,100);
	string input(in);
	Socket socket;
	int ack;
	int i=0;
	window_size--;
	int count = 0;
	while(i<(input.length()+window_size)){
		if(i<input.length()){
			cout<<"Sending "<<input[i]<<" : "<<i<<endl;
			socket.send(input[i],i);
		}
		if(i>=window_size){
			ack = socket.receive(i-window_size);
			if(ack!=INT_MAX){
				if(ack==count){
					cout<<"Received ack : "<<ack<<endl;
					count++;
				}
			}
			else{
				cout<<"Not received ack : "<<i-window_size<<endl;
				i = i - window_size - 1;
			}
		}
		i++;
	}
	cout<<"Received : "+socket.receivedData()<<endl;
	return 0;
}