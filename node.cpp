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
	int start_time[10];
	int msg[10];

	int input_reader()
	{
		int i=0;
		fin[1].open("input.txt");
		string line;
		string t,t1;
		string full_time[10];
		string sec[10];
		string min[10];
		int mins[10],secs[10];
		string message[10];
		while (getline(fin[1], line))											//Read entire line from file
    {
        istringstream ss(line);


        string minute, msg;


				char *cstr = new char[line.length() + 1];
				strcpy(cstr, line.c_str());

				char *token = std::strtok(cstr, "\t");
				full_time[i]=token;
				//cout<<hour[i]<<endl;
    		while (token != NULL)												//Read splitting message and full time
				{
					message[i] = token;
        	token = std::strtok(NULL, "\t");
    		}
				//----------------
				char *temp = new char[full_time[i].length() + 1];
				strcpy(temp,full_time[i].c_str());
				char *token1 = std::strtok(temp, ":");
				min[i]=token1;
				while (token1 != NULL)
				{
					sec[i] = token1;
        	token1 = std::strtok(NULL, ":");							//splitting mins and secs from full time
    		}
				//--------------------
				mins[i]=atoi(min[i].c_str());
				secs[i]=atoi(sec[i].c_str());
				start_time[i]=(mins[i]*60)+secs[i];								//Calculating start time
				i++;

    }
		for(i=0;i<4;i++)
		{
			cout<<full_time[i]<<" "<<message[i]<<" "<<start_time[i]<<endl;
		}
		return 0;
	}

}c[50];
