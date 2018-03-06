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
	client.state=0;
	

	client.input_file=argv[1];
	client.config_file=argv[2];
	client.input_reader();
	client.config_reader();

	int p=client.port+1;


	fout.open("o_"+to_string(client.port)+".txt",ofstream::app);
	
	std::this_thread::sleep_for(std::chrono::seconds(client.join_time));
	client.socket_creator(); 
	cout<<client.port<<" awake"<<endl;
	//client.sender(client.msg[1]);

	client.state=1;
	client.best_candidate=client.port;
	client.prev_id=0;
	client.next_id=0;
	client.probe_permission=1;

	//cout<<"range= "<<client.lower_range<<endl;
	int l=0;
	while(l<50)
	{
		if(client.prev_id>0 && client.next_id>0)
		{
			client.state=2;
			cout<<client.port<<" moved to state 2 in "<<l<<endl;
			l++;
			//continue;
		}
		if(client.next_id>0 && client.prev_id==0)
		{
			client.probe_permission=0;
		}
		//***********************************************************Everyone listens to probe
		if(client.state!=9)
		{
			cout<<client.port<<" searching for probes in "<<l<<endl;
			fd_set fds;
			FD_ZERO(&fds); 
			FD_SET(client.s, &fds);
			struct timeval probe_timeout;
			probe_timeout.tv_sec=2;

			rc = select(client.s+1, &fds, NULL, NULL, &probe_timeout);

			
			if (rc < 0)
			{
				printf("error in select\n");
	            getchar();
			}
			else
			{

				int socket_data=0;
				if (FD_ISSET(client.s,&fds))
				{
					//cerr<<client.port<<" got msg "<<l<<endl;
					client.received_msg=client.receiver();
					char *ch_str = new char[client.received_msg.length() + 1];
					strcpy(ch_str, client.received_msg.c_str());
					char *tk = strtok(ch_str, " ");
					
					string signal(tk);
					string temp;
					//cout<<signal<<endl;
					if(signal=="probe")
					{
						while (tk != NULL)												//Read splitting message and full time
						{
							temp = tk;
							tk = strtok(NULL, " ");
						}

						//string temp1(temp);
						int probe_from=stoi(temp);
						cout<<client.port<<" got probe from "<<probe_from<<" in "<<l<<endl;
						if(probe_from>client.prev_id)
						{
							client.prev_id=probe_from;
							client.send_probe_ack(client.prev_id);
							cout<<client.port<<" sent ack to "<<client.prev_id<<" in "<<l<<endl;

							if(client.state==2)
							{
								client.state=1;
								client.next_id=0;
								client.prev_id=0;
								l++;
								continue;
							}
							//
							
							//l++;
							//continue;

						}

					}
					
				}

			}

		}
		//***********************************************************************probing
		if(client.state==1 && client.probe_permission==1)
		{
			
			//client.sender(client.msg[1],3555);
			if(p<=client.lower_range && p>client.port)
			{
				client.send_probe_request(p);
				cout<<client.port<<" sent probe to upper "<<p<<" in "<<l<<endl;
				

				//-----------------------------------------------------
				fd_set fds;
				FD_ZERO(&fds); 
				FD_SET(client.s, &fds);
				struct timeval probe_timeout;
				probe_timeout.tv_sec=1;

				rc = select(client.s+1, &fds, NULL, NULL, &probe_timeout);
				if (rc < 0)
				{
					printf("error in select\n");
		            getchar();
				}
				else
				{
					int socket_data=0;
					if (FD_ISSET(client.s,&fds))
					{

						//cerr<<client.port<<" got msg "<<l<<endl;
						client.received_msg=client.receiver();
						char *ch_str = new char[client.received_msg.length() + 1];
						strcpy(ch_str, client.received_msg.c_str());
						char *tk = strtok(ch_str, " ");
						
						string signal(tk);
						string temp;
						//cout<<signal<<endl;
						if(signal=="ack")
						{
							while (tk != NULL)												//Read splitting message and full time
							{
								temp = tk;
								tk = strtok(NULL, " ");
							}

							//string temp1(temp);
							client.next_id=stoi(temp);
							cout<<client.port<<" got ack from "<<client.next_id<<" in "<<l<<endl;

							
						}
					}
				//-----------------------------------------------------
				}


			

				p++;
				

			}
			if(p==client.lower_range)
			{
				p=client.upper_range;
				l++;
				continue;
			}
			if(p<client.port)
			{
				client.send_probe_request(p);
				cout<<client.port<<" sent probe to lower "<<p<<" in "<<l<<endl;
				

				//-----------------------------------------------------
				fd_set fds;
				FD_ZERO(&fds); 
				FD_SET(client.s, &fds);
				struct timeval probe_timeout;
				probe_timeout.tv_sec=2;

				rc = select(client.s+1, &fds, NULL, NULL, &probe_timeout);
				if (rc < 0)
				{
					printf("error in select\n");
		            getchar();
				}
				else
				{
					int socket_data=0;
					if (FD_ISSET(client.s,&fds))
					{

						//cerr<<client.port<<" got msg "<<l<<endl;
						client.received_msg=client.receiver();
						char *ch_str = new char[client.received_msg.length() + 1];
						strcpy(ch_str, client.received_msg.c_str());
						char *tk = strtok(ch_str, " ");
						
						string signal(tk);
						string temp;
						//cout<<signal<<endl;
						if(signal=="ack")
						{
							while (tk != NULL)												//Read splitting message and full time
							{
								temp = tk;
								tk = strtok(NULL, " ");
							}

							//string temp1(temp);
							client.next_id=stoi(temp);
							cout<<client.port<<" got ack from "<<client.next_id<<" in "<<l<<endl;
							
							
						}

					}
				//-----------------------------------------------------
				}
				p++;

			}
			if(p==client.port)
			{
				cout<<client.port<<" no one in the ring"<<endl;
				client.state=2;
			}
			
			
		}
		//***********************************************************************Elections
		if(client.state==2)
		{

			std::this_thread::sleep_for(std::chrono::seconds(1));
			//cout<<client.port<<" started elections in "<<l<<endl;
			client.send_candidate(client.next_id);
			fout<<client.next_id<<endl;
			cout<<client.port<<" sent "<<client.best_candidate<<" as candidate in "<<l<<endl;

			fd_set fds;
			FD_ZERO(&fds); 
			FD_SET(client.s, &fds);
			struct timeval candidate_timeout;
			candidate_timeout.tv_sec=2;

			rc = select(client.s+1, &fds, NULL, NULL, &candidate_timeout);
			if (rc < 0)
			{
				printf("error in select\n");
	            getchar();
			}
			else
			{
				int socket_data=0;
				if (FD_ISSET(client.s,&fds))
				{

					//cerr<<client.port<<" got msg "<<l<<endl;
					client.received_msg=client.receiver();
					char *ch_str = new char[client.received_msg.length() + 1];
					strcpy(ch_str, client.received_msg.c_str());
					char *tk = strtok(ch_str, " ");
					
					string signal(tk);
					string temp;
					//cout<<signal<<endl;
					if(signal=="candidate1")
					{
						cout<<client.port<<" got candidate message from "<<client.prev_id<<" in "<<l<<endl;
						while (tk != NULL)												
						{
							temp = tk;
							tk = strtok(NULL, " ");
						}

						//string temp1(temp);
						if(client.best_candidate < stoi(temp))
						{
							client.best_candidate=stoi(temp);
						}
						
						cout<<client.port<<" best candidate is "<<client.best_candidate<<" in "<<l<<endl;
						//client.state=2;
						l++;
						continue;
						
					}
				}
			//-----------------------------------------------------
			}
		}


		//*********************************************************************posting

		l++;
		
	}//---------------------------while ends-----------------------------------

	
	
}
