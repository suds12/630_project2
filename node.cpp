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
#include<cstring>
#include <chrono>
#include <thread>
#define MAX_MESSAGE_LEN 65536


using namespace std;
//using namespace std::this_thread; // sleep_for, sleep_until
//using namespace std::chrono; // nanoseconds, system_clock, seconds
typedef map<string,struct sockaddr_in> channel_type;
struct sockaddr_in sender;
struct sockaddr_in receiver;
struct sockaddr_in recv_client;

ifstream fin[50];
ofstream fout[5];

struct timeval tv;

class client
{
public:
	string message;
	int port;
	int next_id;
	int prev_id;
	int message_time[10];
	string msg[10];
	int join_time;
	int leave_time;
	int range;
	int uid;
	//int s;

	int input_reader()
	{
		int i=0;
		fin[0].open("input.txt");
		string line;
		string t,t1;
		string full_time[10];
		string sec[10];
		string min[10];
		int mins[10],secs[10];
		string message[10];
		while (getline(fin[0], line))											//Read entire line from file
    {
        istringstream ss(line);
        string minute;

				char *cstr = new char[line.length() + 1];
				strcpy(cstr, line.c_str());

				char *token = std::strtok(cstr, "\t");
				full_time[i]=token;
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
				message_time[i]=(mins[i]*60)+secs[i];								//Calculating start time
				msg[i]=message[i];
				i++;

    }
		for(i=0;i<4;i++)
		{
			//cout<<full_time[i]<<" "<<msg[i]<<" "<<message_time[i]<<endl;
		}
		return 0;
	}
//**************************************************************************
	int config_reader()
	{
		int i=0,j=0;
		fin[uid].open("config_"+to_string(uid)+".txt");
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
		int upper_range;
		int lower_range;
		int temp_join_hour;
		int temp_join_min;
		int temp_leave_hour;
		int temp_leave_min;


		while (getline(fin[uid], line))
		{
			istringstream ss(line);
			char *cstr = new char[line.length() + 1];
			strcpy(cstr, line.c_str());
			char *token = std::strtok(cstr, ":");


			while (token != NULL)												//Read splitting message and full time
			{
				if(i==0)
				{
					//cerr<<token<<endl;
					temp_range = token;
					token = std::strtok(NULL, ":");
				}
				if(i==1)
				{
					//cerr<<token<<endl;
					temp_port = token;
					token = std::strtok(NULL, ":");

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

					token = std::strtok(NULL, ":");
					
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

					token = std::strtok(NULL, ":");

				}
			}
			i++;
		}
		char *temp = new char[temp_range.length() + 1];
		strcpy(temp,temp_range.c_str());
		char *token1 = std::strtok(temp, "-");
		temp_upper_range=token1;
		while (token1 != NULL)
		{
			temp_lower_range = token1;
			token1 = std::strtok(NULL, ":");							//splitting mins and secs from full time
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
	//**************************************************************************
	void initialize()
	{
		int i,j;
		//sleep_for(seconds(10));
		//sleep_until(system_clock::now() + seconds(join_time));
		//cout<<"hi "<<uid<<endl;
	}
	//**************************************************************************

	void accumulate()
	{
		input_reader();
		//config_reader();
		initialize();
	}
	//**************************************************************************

	void send_msg()
	{
		int i,rc,s;
		int temp=3452;
		char message[1000];
		size_t len;
		ssize_t bytes;


		strcpy(message, msg[2].c_str());
		len = sizeof message;
		//cerr<<len<<endl;
		s = socket(PF_INET, SOCK_DGRAM, 0);
		if (s < 0)
		{
			perror ("socket() failed\n");
			exit(1);
		}
		receiver.sin_family = AF_INET;
		receiver.sin_port = htons(temp);
		cerr<<receiver.sin_port<<endl;

		bytes = sendto(s, message, len, 0, (struct sockaddr*)&receiver, sizeof receiver);
		
		//cerr<<message<<endl;
	}

	//**************************************************************************
	void recv_msg()
	{
		int i,rc,s;
		tv.tv_sec=2;
		int temp=3452;
		char recv_message[1000];
		size_t len;
		ssize_t bytes;
		socklen_t fromlen;
		fromlen = sizeof(sender);
		//cerr<<fromlen<<endl;
		s = socket(PF_INET, SOCK_DGRAM, 0);
		if (s < 0)
		{
			perror ("socket() failed\n");
			exit(1);
		}
		recv_client.sin_family = AF_INET;
		recv_client.sin_port = htons(temp);
		recv_client.sin_addr.s_addr = INADDR_ANY;
		int err;
		err = bind(s, (struct sockaddr*)&recv_client, sizeof(recv_client));
		if (err < 0)
		{
		  perror("bind failed\n");
		}
		else
		{
		  printf("bound socket\n");
		}
		//cerr<<sizeof(recv_client)<<endl;
		cerr<<recv_client.sin_port<<endl;
		i=0;
		while(i<1) //server runs for ever
		{
		  
		  fd_set fds;
		  FD_ZERO(&fds);
		  FD_SET(s, &fds);

		  rc = select(s+1, &fds, NULL, NULL, &tv);
		  
		  if (rc < 0)
		  {
		    printf("error in select\n");
		          getchar();
		  }
		  else
		  {
		    int socket_data = 0;
		    cerr<<rc<<endl;
		    if (FD_ISSET(s,&fds))
		    {

		    	cerr<<"isset"<<endl;
		      //reading from socket

		      bytes = recvfrom(s, recv_message, sizeof(recv_message), 0, (struct sockaddr*)&sender, &fromlen);
		      
		      socket_data = 1;
		      break;

		    }

		  }
		  i++;
		}





	}


}c[50];
