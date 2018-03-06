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
	probe="probe "+to_string(port);
	sender(probe,p);
	//cout<<"sent probe to "<<p<<endl;

}

void client_process::send_probe_ack(int p)
{
	int i;
	string ack;
	//cout<<"done"<<endl;
	ack="ack "+to_string(port);
	sender(ack,p);
	
	
}

