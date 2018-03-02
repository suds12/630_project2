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

client_process::socket_creator()
{
  int i,rc,s;
	ssize_t bytes;
  size_t len;
	char msg_comm[1000];
  socklen_t fromlen;
	fromlen = sizeof(my_sock);
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
