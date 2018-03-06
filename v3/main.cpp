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
#include "election.cpp"
#define MAX_MESSAGE_LEN 65536


int main(int argc, char** argv)
{

	int rc;

	client.input_file=argv[1];
	client.config_file=argv[2];
	client.input_reader();
	client.config_reader();
	int p=client.port+1;
	fout.open("o_"+to_string(client.port)+".txt",ofstream::app);

	std::this_thread::sleep_for(std::chrono::seconds(client.join_time));
	client.socket_creator(); 
	cout<<client.port<<" awake"<<endl;

	client.init();
	int l=0;

	client.send_probe_request(p+1);
	if(client.port==3559)
	{
		std::this_thread::sleep_for(std::chrono::seconds(8));
		client.send_probe_request(3553);
		cout<<client.port<<" sent probe"<<endl;
	}

	while(1)
	{
		fd_set fds;
		FD_ZERO(&fds); 
		FD_SET(client.s, &fds);
		struct timeval overall_timeout;
		overall_timeout.tv_usec=50;

		rc = select(client.s+1, &fds, NULL, NULL, &overall_timeout);
		if (rc < 0)
		{
			printf("error in select\n");
            getchar();
		}
		else
		{
			if (FD_ISSET(client.s,&fds))
			{
				if(client.state==1)//*************start of state 1*******************************
				{	
					client.received_msg=client.receiver();
					char *ch_str = new char[client.received_msg.length() + 1];
					strcpy(ch_str, client.received_msg.c_str());
					char *tk = strtok(ch_str, "::");

					string signal(tk);
					string temp;
					//cout<<signal<<endl;
					if(signal=="ack")
					{
						while (tk != NULL)										
						{
							temp = tk;
							tk = strtok(NULL, "::");
						}
						client.next_id=stoi(temp);
						cout<<client.port<<" got ack from "<<client.next_id<<" in "<<l<<endl;
						client.state=2;
						client.send_candidate(client.next_id);
						cout<<client.port<<" sent candidate to "<<client.next_id<<" in "<<l<<endl;


					}

					else if(signal=="probe1")
					{
						//cerr<<client.port<<" entered cs1"<<endl;
						while (tk != NULL)												
						{
							temp = tk;
							tk = strtok(NULL, "::");
						}

						//string temp1(temp);
						int probe_from=stoi(temp);
						cout<<client.port<<" got probe from "<<probe_from<<" in "<<l<<endl;
						if(probe_from>client.prev_id)
						{
							client.prev_id=probe_from;
							client.send_probe_ack(client.prev_id);
							cout<<client.port<<" sent ack to "<<client.prev_id<<" in "<<l<<endl;
						}
					}
				}//***********************************end of state 1***********************************
				
				if(client.state==2)//**********************************start of state 2******************************
				{
					client.received_msg=client.receiver();
					char *ch_str = new char[client.received_msg.length() + 1];
					strcpy(ch_str, client.received_msg.c_str());
					char *tk = strtok(ch_str, "::");

					string signal(tk);
					string temp;
					string word[5];
					int k=0;

					if(signal=="probe1")
					{
						//cerr<<client.port<<" entered cs1"<<endl;
						while (tk != NULL)												
						{
							temp = tk;
							tk = strtok(NULL, "::");
						}

						//string temp1(temp);
						int probe_from=stoi(temp);
						cout<<client.port<<" got probe from "<<probe_from<<" in "<<l<<endl;
						if(probe_from>client.prev_id)
						{
							client.prev_id=probe_from;
							client.send_probe_ack(client.prev_id);
							cout<<client.port<<" sent ack to "<<client.prev_id<<" in "<<l<<endl;
						}
					}

					else if(signal=="candidate1")
					{
						while (tk != NULL)												
						{
							word[k] = tk;
							tk = strtok(NULL, "::");
							k++;
						}
						
						client.proposed_candidate=stoi(word[1]);
						client.seqno=stoi(word[2]);

						if(client.port==client.proposed_candidate)
						{
							cout<<client.port<<" I am leader "<<endl;
							client.send_leader(client.next_id);
							//client.state=3;
						}
						else if(client.port<client.proposed_candidate)
						{
							client.best_candidate=client.proposed_candidate;
							cout<<client.port<<" passing "<<client.best_candidate<<endl;
							client.pass_candidate(client.next_id, client.best_candidate, client.seqno);
						}
						else if(client.port>client.proposed_candidate)
						{
							client.best_candidate=client.port;
							client.pass_candidate(client.next_id, client.best_candidate, client.seqno);
						}
						
						
					}

					else if(signal=="leader1")
					{
						while (tk != NULL)												
						{
							word[k] = tk;
							tk = strtok(NULL, "::");
							k++;
						}
						client.leader=stoi(word[1]);
						cout<<client.port<<" leader is "<<client.leader<<endl;
						client.pass_leader(client.next_id, client.leader);
					}
				}//**********************************end of state 2******************************
				
			}
		
		}
		//l++;

	}//-----------------------------------------------------end of while----------
}