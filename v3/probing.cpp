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



void client_process::send_probe_request(int p)
{
	int i;
	string probe;
	//cout<<"done"<<endl;
	probe="probe1::"+to_string(port);
	sender(probe,p);
	//cout<<"sent probe to "<<p<<endl;

}

void client_process::send_probe_ack(int p)
{
	int i;
	string ack;
	//cout<<"done"<<endl;
	ack="ack::"+to_string(port);
	sender(ack,p);
	
}

void client_process::init()
{
	state=1;
	best_candidate=0;
	proposed_candidate=0;
	prev_id=0;
	next_id=0;
	probe_permission=1;

}

