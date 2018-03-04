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



void client_process::send_probe_request()
{
	int i;
	string probe;
	cout<<"done"<<endl;
	probe="probe "+to_string(port);
	cout<<probe<<endl;
	for(i=0;i<range;i++)
	{
		sender(probe);
	}
}

