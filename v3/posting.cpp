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

void client_process::gen_token()
{
	token=1;
}

void client_process::send_token(int var1)
{
	string tk="token1";
	token=0;
	sender(tk, var1);

}

void client_process::send_post(int var1)
{
	
	clock_t c_time = clock(); 
	double read_time=double(c_time - begin)/CLOCKS_PER_SEC;
	//cout<<port<<" read_time "<<message_time[0]<<endl;
	if(token==1)
	{
		if(read_time>message_time[bookmark])
		{
			cout<<client.port<<" posting "<<msg[bookmark]<<endl;
			sender(msg[bookmark], var1);
		}

		bookmark++;
	}
}

void client_process::display_time()
{
	clock_t time_now=clock();
	double time_since=double(time_now - begin)/CLOCKS_PER_SEC;
	cout<<time_now;

}