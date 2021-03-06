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

class client_process
{
public:
	string message;
	int port;
	int next_id;
	int prev_id;
	int state;
	int message_time[10];
	string msg[10];
	int join_time;
	int leave_time;
	int upper_range;
	int lower_range;
	int range;
	int s;
	string input_file;
	string config_file;
	string received_msg;

	int input_reader()
	{
		int i=0;
		fin[1].open(input_file.c_str());
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
			string minute;

			char *cstr = new char[line.length() + 1];
			strcpy(cstr, line.c_str());

			char *token = strtok(cstr, "\t");
			full_time[i]=token;
    		while (token != NULL)												//Read splitting message and full time
    		{
    			message[i] = token;
    			token = strtok(NULL, "\t");
    		}
				//----------------
    		char *temp = new char[full_time[i].length() + 1];
    		strcpy(temp,full_time[i].c_str());
    		char *token1 = strtok(temp, ":");
    		min[i]=token1;
    		while (token1 != NULL)
    		{
    			sec[i] = token1;
        		token1 = strtok(NULL, ":");							//splitting mins and secs from full time
        	}
				//--------------------
       		mins[i]=atoi(min[i].c_str());
        	secs[i]=atoi(sec[i].c_str());
			message_time[i]=(mins[i]*60)+secs[i];								//Calculating start time
			msg[i]=message[i];
			i++;

		}

		for(i=0;i<4;i++)
		{
			//cout<<full_time[i]<<" "<<message[i]<<" "<<message_time[i]<<endl;
		}

		return 0;
	}

		int config_reader()
		{
			int i=0,j=0;
			fin[2].open(config_file);
			string temp_range;
			string temp_port;
			string temp_join_time;
			string temp_leave_time;
			string join_hour;
			string join_min;
			string leave_hour;
			string leave_min;
			string line;
			string temp_upper_range;
			string temp_lower_range;
			int temp_join_hour;
			int temp_join_min;
			int temp_leave_hour;
			int temp_leave_min;

			while (getline(fin[2], line))
			{
				istringstream ss(line);
				char *cstr = new char[line.length() + 1];
				strcpy(cstr, line.c_str());
				char *token = strtok(cstr, ":");

			while (token != NULL)												//Read splitting message and full time
			{
				if(i==0)
				{
					//cerr<<token<<endl;
					temp_range = token;
					token = strtok(NULL, ":");
				}
				if(i==1)
				{
					//cerr<<token<<endl;
					temp_port = token;
					token = strtok(NULL, ":");

				}
				if(i==2)
				{
					//cerr<<token<<endl;
					//temp_join_time = token;
					if(join_hour =="")
					{
						join_hour=token;
					}
					if(join_hour =="join_time")
					{
						join_hour=token;
					}
					else
						join_min=token;

					token = strtok(NULL, ":");
					//cerr<<temp_join_time<<endl;
				}
				j=0;
				if(i==3)
				{
					if(leave_hour =="")
					{
						leave_hour=token;
					}
					if(leave_hour =="leave_time")
					{
						leave_hour=token;
					}
					else
						leave_min=token;

					token = strtok(NULL, ":");

				}
			}
			i++;
		}
		char *temp = new char[temp_range.length() + 1];
		strcpy(temp,temp_range.c_str());
		char *token1 = strtok(temp, "-");
		temp_upper_range=token1;
		while (token1 != NULL)
		{
			temp_lower_range = token1;
			token1 = strtok(NULL, ":");							//splitting mins and secs from full time
		}

  	//------------------------------------

		upper_range=atoi(temp_upper_range.c_str());
		lower_range=atoi(temp_lower_range.c_str());
		range=lower_range-upper_range;
		port=atoi(temp_port.c_str());
		temp_join_hour=atoi(join_hour.c_str());
		temp_join_min=atoi(join_min.c_str());
		temp_leave_hour=atoi(leave_hour.c_str());
		temp_leave_min=atoi(leave_min.c_str());

		join_time=(temp_join_hour*60)+temp_join_min;
		leave_time=(temp_leave_hour*60)+temp_leave_min;


		//cout<<range<<" "<<port<<" "<<join_time<<" "<<leave_time<<endl;
		return 0;
	}

	void initializer();
	void socket_creator();
	void sender(string var1,int var2);
	string receiver();
	//void receiver_old();
	void send_probe_request(int p);
	void send_probe_ack(int p);



}client;
