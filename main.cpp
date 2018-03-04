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
//-------
using namespace std;



#include "reader.cpp"
#include "communication.cpp"
#include "probing.cpp"
#define MAX_MESSAGE_LEN 65536






int main(int argc, char** argv)
{
	
	int rc;

	client.input_file=argv[1];
	client.config_file=argv[2];
	client.input_reader();
	client.config_reader();
	fout.open("o_"+to_string(client.port)+".txt",ofstream::app);
	client.socket_creator(); 

	std::this_thread::sleep_for(std::chrono::seconds(client.join_time));
	cout<<client.port<<" awake"<<endl;
	client.sender(client.msg[1]);
	client.send_probe_request();
	//cout<<"sent "<<client.port<<endl;
	while(1)
	{
		fd_set fds;
		FD_ZERO(&fds); 
		FD_SET(client.s, &fds);

		rc = select(client.s+1, &fds, NULL, NULL, NULL);
		if (rc < 0)
		{
			printf("error in select\n");
            getchar();
		}
		else
		{
			int socket_data=0;
			if (FD_ISSET(client.s,&fds))
			{

				client.receiver();

				socket_data = 1;
			}
			
			

		}
		
		//cout<<"next "<<client.port<<endl;
		
	}
	
	
}
