#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<cstdlib>
#include<string.h>
#include<sstream>
#include<ctime>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<map>
#include<thread>
#include<chrono>
#define MAX_MESSAGE_LEN 65536

using namespace std;



void client_process::send_candidate(int dest)
{
	int i;
	string candidate;
	//cout<<"done"<<endl;
	candidate="candidate1::"+to_string(port)+"::"+to_string(port);
	sender(candidate,dest);
	//cout<<"sent probe to "<<p<<endl;
}

void client_process::pass_candidate(int dest, int proposed, int seq)
{
	int i;
	string candidate;
	//cout<<"done"<<endl;
	candidate="candidate1::"+to_string(proposed)+"::"+to_string(seq);
	sender(candidate,dest);
	//cout<<"sent probe to "<<p<<endl;
}

void client_process::send_leader(int dest)
{
	int i;
	string candidate;
	//cout<<"done"<<endl;
	candidate="leader1::"+to_string(port)+"::"+to_string(port);
	sender(candidate,dest);
	//cout<<"sent probe to "<<p<<endl;
}

void client_process::pass_leader(int dest, int leader)
{
	int i;
	string candidate;
	//cout<<"done"<<endl;
	candidate="leader1::"+to_string(leader);
	sender(candidate,dest);
}





