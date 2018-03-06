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
#define port 6000

using namespace std;

struct sockaddr_in serv_addr;

int main()
{
	int i,rc,s;
	s = socket(PF_INET, SOCK_DGRAM, 0);
	if (s < 0)
	{
		perror ("socket() failed\n");
		exit(1);
	}
	void *data;
	size_t len;
	ssize_t bytes;
	char msg[100];
	string text="rebel berry";
	//data = &msg;
	strcpy(msg, text.c_str());
	len = sizeof msg;

	serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

	bytes = sendto(s, msg, len, 0, (struct sockaddr*)&serv_addr, sizeof serv_addr);



}
	
