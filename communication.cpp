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
struct sockaddr_in my_sock;
struct sockaddr_in next_sock;


void client_process::socket_creator()
{
  int i,rc;
	
  s = socket(PF_INET, SOCK_DGRAM, 0);

	if (s < 0)
	{
		perror ("socket() failed\n");
		exit(1);
	}
	cerr<<endl<<"socket created";

	my_sock.sin_family = AF_INET;
	my_sock.sin_port = htons(port);
	my_sock.sin_addr.s_addr = INADDR_ANY;
	int err;
	err = bind(s, (struct sockaddr*)&my_sock, sizeof my_sock);
	if (err < 0)
	{
		perror("bind failed\n");
	}
	else
	{
		printf("bound socket\n");
	}	
}

void client_process::sender()
{
	
	ssize_t bytes;
  	size_t len;
	char msg_comm[1000];
	strcpy(msg_comm, msg[1].c_str());

	//len= msg[1].length();
	//cout<<msg[1]<<endl;
	next_sock.sin_family = AF_INET;
    next_sock.sin_port = htons(3452);
	bytes = sendto(s, msg_comm, 100, 0, (struct sockaddr*)&next_sock, sizeof next_sock);
	cerr<<"sent " << bytes << endl;
}

void client_process::receiver()
{
	int rc;
	int k=0;
	struct timeval tv;
	tv.tv_sec=2;
	ssize_t bytes;
	void *data;
	size_t len;
	char msg[100];
	socklen_t fromlen;
	fromlen = sizeof(my_sock);

	while(1)
	{
		
		fd_set fds;
		FD_ZERO(&fds);
		FD_SET(s, &fds);
		
		//cout << "before select" << endl;

		rc = select(s+1, &fds, NULL, NULL, &tv);
		//cout << "after select" << endl;
		

		
		if (rc < 0)
		{
			printf("error in select\n");
            getchar();
		}
		else

			if (FD_ISSET(s,&fds))
			{


				cout<< "Rc: " << rc<<endl;
				bytes = recvfrom(s, msg, sizeof(msg), 0, (struct sockaddr*)&my_sock, &fromlen);
				cerr<<"yooo " << bytes<<endl;
				cout<<msg;
			}

		k++;

	}
}