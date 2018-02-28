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
		int i=0;
		fin[1].open("input.txt");
		string line;
		string t,t1;
		string full_time[10];
		string message[10];
		while (getline(fin[1], line))
    {
        istringstream ss(line);

				//cout<<line;

        string minute, msg, sec,min;

        //ss >> minute >> sec >> msg;
        //cout << minute << ":"<<endl;
				//-----------------
				//cout<<line<<endl;
				//ss>>minute;
				//cout<<minute<<endl;
				//char const *read = line.c_str();
				//cerr<<read<<endl;
				//cout<<"start"<<endl;
				char *cstr = new char[line.length() + 1];
				strcpy(cstr, line.c_str());

				char *token = std::strtok(cstr, "\t");
				//cout<<token<<endl;
				full_time[i]=token;
				//cout<<hour[i]<<endl;
    		while (token != NULL)
				{
        	//std::cout << token << '\n';
					//cout<<"gap"<<endl;
					message[i] = token;
					//cout<<message[i]<<endl;
        	token = std::strtok(NULL, "\t");
					//std::cout << "done" << '\n';
					//message[i] += token;

    		}
				i++;

    }
		for(i=0;i<4;i++)
		{
			cout<<full_time[i]<<" "<<message[i]<<endl;
		}
		return 0;
	}

}c[50];
