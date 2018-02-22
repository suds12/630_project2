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
#define MAX_MESSAGE_LEN 65536

using namespace std;

typedef map<string,struct sockaddr_in> channel_type;
struct sockaddr_in server;
struct sockaddr_in recv_client;

class client
{
public:
	string message;
	int port;
	int next_id;
	int prev_id;

}c[50];



int main()
{
	int i,rc,s;
	ssize_t bytes;
	void *data;
	size_t len;
	char msg[100];
	socklen_t fromlen;
	fromlen = sizeof(recv_client);
	//-------------------------------------------socket creation and binding
	s = socket(PF_INET, SOCK_DGRAM, 0);

	if (s < 0)
	{
		perror ("socket() failed\n");
		exit(1);
	}
	//cerr<<endl<<"socket created";

	int port=6000;
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = INADDR_ANY;	
	int err;
	err = bind(s, (struct sockaddr*)&server, sizeof server);
	if (err < 0)
	{
		perror("bind failed\n");
	}
	else
	{
		printf("bound socket\n");
	}
	//----------------------------------------------------

	while(1) //server runs for ever
	{
		int rc;
		fd_set fds;
		FD_ZERO(&fds);
		FD_SET(s, &fds);
		
		//cout << "before select" << endl;

		rc = select(s+1, &fds, NULL, NULL, NULL);
		//cout << "after select" << endl;
		

		
		if (rc < 0)
		{
			printf("error in select\n");
            getchar();
		}
		else
		{
			int socket_data = 0;
			/*
			if (FD_ISSET(0,&fds))
			{
               
				//reading from socket
				cerr<<"Heyyy";
				socket_data = 1;

			}
			*/
			if (FD_ISSET(s,&fds))
			{
               
				//reading from socket
				bytes = recvfrom(s, msg, sizeof(msg), 0, (struct sockaddr*)&recv_client, &fromlen);
				cout<< msg<< endl;
				socket_data = 1;
				break;

			}
			
		}	
	}
}
