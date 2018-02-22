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
ifstream fin[5];
ofstream fout[5];

class client
{
public:
	string message;
	int port;
	int next_id;
	int prev_id;

	int reader()
	{
		fin[1].open("input.txt");
		string line;
		while (getline(fin[1], line,':'))
    {
        istringstream ss(line);

        string msg;
        string minute, sec;

        ss >> minute >> sec >> msg;
        cout << minute << ":"<<sec <<" "<<msg<<endl;
    }
		return 0;
	}

}c[50];
