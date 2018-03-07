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
#include "posting.cpp"
#define MAX_MESSAGE_LEN 65536


int main(int argc, char** argv)
{

	if(argc != 7) {
        cout << "Invalid Argument: ./ring -c <cfg.txt> -i <in.txt> -o <out.txt>" << endl;
        exit(0);
    }
    for(int i=1; i<argc; i+=2) {
        if (i+1 != argc) {
            if (strcmp(argv[i], "-c") == 0) {
                client.config_file = argv[i+1];
                //cout << client.config_file << endl;
            }

            else if(strcmp(argv[i], "-i") == 0) {
                client.input_file = argv[i+1];
                //cout << client.input_file << endl;
            }
            else if(strcmp(argv[i], "-o") == 0)
                client.output_file = argv[i+1];
            else {
                cout << "Invalid arguments! Usage: ./ring -c <cfg.txt> -i <in.txt> -o <out.txt>";
                exit(0);
            }
        }
    }
    if (client.config_file=="" || client.input_file=="" || client.output_file=="") {
        cout << "Invalid arguments" << endl;
        exit(0);
    }

	int rc;
	//clock_t display_time=clock();
	double elapsed_min;
	double elapsed_sec;
	auto display_time = std::chrono::high_resolution_clock::now();
	auto end = std::chrono::high_resolution_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - display_time);

	client.begin = std::chrono::high_resolution_clock::now();
	//client.begin = clock();	//start clock

	client.input_reader();
	client.config_reader();
	int p=client.port+1;
	fout.open(client.output_file,ofstream::out);

	std::this_thread::sleep_for(std::chrono::seconds(client.join_time));
	client.socket_creator(); 
	cout<<client.port<<" awake"<<endl;
	

	client.init();
	int l=0;


	client.next_probe++;
	client.send_probe_request(client.next_probe);
	cout<<client.port<<" sent probe to "<<client.next_probe<<endl;
	/*
	if(client.port==3559)
	{
		std::this_thread::sleep_for(std::chrono::seconds(8));
		client.send_probe_request(3553);
		cout<<client.port<<" sent probe"<<endl;
	}
	*/
	
	client.track_time = std::chrono::high_resolution_clock::now();
	//client.track_time = clock();
	while(1)
	{
		fd_set fds;
		FD_ZERO(&fds); 
		FD_SET(client.s, &fds);
		struct timeval overall_timeout;
		overall_timeout.tv_usec=50000;

		auto current_time = std::chrono::high_resolution_clock::now();
		auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - client.begin).count();
				
		if(elapsed_time>client.leave_time*1000)
		{
			client.state=5;
			cout << client.port << " says BYEEE" << endl;
			exit(0);
		}

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
						//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
						end = std::chrono::high_resolution_clock::now();
						elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - display_time);
						elapsed_min=elapsed.count()/60;
						elapsed_sec=elapsed.count()%60;

						fout<<elapsed_min<<":"<<elapsed_sec<<" next hop is changed to client "<<client.next_id<<endl;
						//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

						
						
						//cout<<client.port<<" got ack from "<<client.next_id<<" in "<<l<<endl;
						client.state=2;
						
						//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
						end = std::chrono::high_resolution_clock::now();
						elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - display_time);
						elapsed_min=elapsed.count()/60;
						elapsed_sec=elapsed.count()%60;

						fout<<elapsed_min<<":"<<elapsed_sec<<" started election, send election message to client "<<client.next_id<<endl;
						//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
						
						client.send_candidate(client.next_id);
						cout<<client.port<<" sent candidate to "<<client.next_id<<" in "<<l<<endl;
						client.track_time = std::chrono::high_resolution_clock::now();
						// client.track_time = clock();

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
						//if(probe_from>=client.prev_id)
						if (((client.port+client.range-probe_from)%client.range <= (client.port+client.range-client.prev_id)%(client.range))
							|| (probe_from != client.port && client.port==client.prev_id))
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
							//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
							end = std::chrono::high_resolution_clock::now();
							elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - display_time);
							elapsed_min=elapsed.count()/60;
							elapsed_sec=elapsed.count()%60;

							fout<<elapsed_min<<":"<<elapsed_sec<<" leader selected "<<endl;
							//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
							
							client.state=3;
							
						}
						else if(client.port<client.proposed_candidate)
						{
							client.best_candidate=client.proposed_candidate;
							cout<<client.port<<" passing "<<client.best_candidate<<endl;
							client.pass_candidate(client.next_id, client.best_candidate, client.seqno);
							//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
							end = std::chrono::high_resolution_clock::now();
							elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - display_time);
							elapsed_min=elapsed.count()/60;
							elapsed_sec=elapsed.count()%60;

							fout<<elapsed_min<<":"<<elapsed_sec<<" relayed election message, leader: client "<<client.best_candidate<<endl;
							//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
						}
						else if(client.port>client.proposed_candidate)
						{
							client.best_candidate=client.port;
							client.pass_candidate(client.next_id, client.best_candidate, client.seqno);
							//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
							end = std::chrono::high_resolution_clock::now();
							elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - display_time);
							elapsed_min=elapsed.count()/60;
							elapsed_sec=elapsed.count()%60;

							fout<<elapsed_min<<":"<<elapsed_sec<<" relayed election message, replaced leader "<<client.best_candidate<<endl;
							//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
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
						
						client.state=4;
					}
					client.track_time = std::chrono::high_resolution_clock::now();
					//client.track_time = clock();
				}//**********************************end of state 2******************************

				if(client.state==3)//***************start of state 3*******************
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

					if(signal=="leader1")
					{
						cout<<"*****"<<endl;
						client.gen_token();
						srand (time(NULL));
						client.token_id=rand() % 1000 + 1;
						//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
						end = std::chrono::high_resolution_clock::now();
						elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - display_time);
						elapsed_min=elapsed.count()/60;
						elapsed_sec=elapsed.count()%60;

						fout<<elapsed_min<<":"<<elapsed_sec<<" new token generated "<<client.token_id<<endl;
						//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
						//client.send_post(client.next_id);
						// c_time = clock(); 
						auto read_time = std::chrono::duration_cast<std::chrono::seconds>(end - client.begin).count();
						//double read_time=double(c_time - client.begin)/CLOCKS_PER_SEC;
						//cout<<port<<" read_time "<<message_time[0]<<endl;
						if(client.token==1)
						{
							//cout << "leader1: read_time " << read_time << " msg time: " <<  client.bookmark << " num msg " << client.num_messages << endl;
							if((client.num_messages != client.bookmark -1) && read_time>client.message_time[client.bookmark])
							{
								string post_msg="post1::"+to_string(client.port)+"::"+client.msg[client.bookmark];
								cout<<client.port<<" posting "<<client.msg[client.bookmark]<<endl;
								client.sender(post_msg, client.next_id);
								client.track_time = std::chrono::high_resolution_clock::now();
								//client.track_time = clock();
								//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
								end = std::chrono::high_resolution_clock::now();
								elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - display_time);
								elapsed_min=elapsed.count()/60;
								elapsed_sec=elapsed.count()%60;

								fout<<elapsed_min<<":"<<elapsed_sec<<" post "<<post_msg<<" was sent "<<endl;
								//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
								
								client.bookmark++;
								
							}
							else
							{
								client.send_token(client.next_id);
								if (client.token_sent == 0)
								{
									//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
									end = std::chrono::high_resolution_clock::now();
									elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - display_time);
									elapsed_min=elapsed.count()/60;
									elapsed_sec=elapsed.count()%60;

									fout<<elapsed_min<<":"<<elapsed_sec<<" token "<<client.token_id<<" was sent to client "<<client.next_id<<endl;
									client.token_sent = 1;
								}

							}
							
						}

						client.state=4;

					}

				}//*****************************end of state 3**********************************

				if(client.state==4)//***********start of state 4*************************
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

					else if(signal=="token1")
					{
						//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
						if (client.token_received == 0)
						{
							client.token_received = 1;
							end = std::chrono::high_resolution_clock::now();
							elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - display_time);
							elapsed_min=elapsed.count()/60;
							elapsed_sec=elapsed.count()%60;

							fout<<elapsed_min<<":"<<elapsed_sec<<" token "<<client.token_id<<" was received "<<endl;
						}
						//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
						client.token=1;
						clock_t c_time = clock(); 
						end = std::chrono::high_resolution_clock::now();
						auto read_time = std::chrono::duration_cast<std::chrono::seconds>(end - client.begin).count();
						//double read_time=double(c_time - client.begin)/CLOCKS_PER_SEC;
						if(client.token==1)
						{
							//cout << "token: read_time " << read_time << " msg time: " <<  client.bookmark << " num msg " << client.num_messages << endl;
							//cout << "read_time " << read_time << " msg time: " <<  client.message_time[client.bookmark] << endl;

							if((client.num_messages != client.bookmark -1) && read_time>client.message_time[client.bookmark])
							{
								string post_msg="post1::"+to_string(client.port)+"::"+client.msg[client.bookmark];
								cout<<client.port<<" posting "<<client.msg[client.bookmark]<<endl;
								client.sender(post_msg, client.next_id);
								client.track_time = std::chrono::high_resolution_clock::now();
								//client.track_time = clock();
								//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
								end = std::chrono::high_resolution_clock::now();
								elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - display_time);
								elapsed_min=elapsed.count()/60;
								elapsed_sec=elapsed.count()%60;

								fout<<elapsed_min<<":"<<elapsed_sec<<" post "<<post_msg<<" was sent "<<endl;
								//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
								if(client.num_messages != client.bookmark -1)
								{
									//client.state=5;
								}
								
									client.bookmark++;
							}
							else
							{
								if (client.token_sent == 0)
								{
									//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
									end = std::chrono::high_resolution_clock::now();
									elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - display_time);
									elapsed_min=elapsed.count()/60;
									elapsed_sec=elapsed.count()%60;

									fout<<elapsed_min<<":"<<elapsed_sec<<" token "<<client.token_id<<" was sent to client "<<client.next_id<<endl;
									client.token_sent = 1;
								}
								client.send_token(client.next_id);
								//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
							}
						}

					}

					else if(signal=="post1")
					{
						while (tk != NULL)												
						{
							word[k] = tk;
							tk = strtok(NULL, "::");
							k++;
						}
						
						client.pid=stoi(word[1]);
						string post_msg=word[2];

						cout<<client.port<<" received post from "<<client.pid<<" :"<<post_msg<<endl;

						if(client.port != client.pid)
						{
							string post_msg="post1::"+to_string(client.pid)+"::"+client.msg[client.bookmark];
							cout<<client.port<<" passing message :"<<post_msg<<endl;
							client.sender(post_msg, client.next_id);
							//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
							end = std::chrono::high_resolution_clock::now();
							elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - display_time);
							elapsed_min=elapsed.count()/60;
							elapsed_sec=elapsed.count()%60;

							fout<<elapsed_min<<":"<<elapsed_sec<<" post "<<post_msg<<" from client "<<client.pid<<" was relayed  "<<endl;
							//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
						}
						else
						{
							cout<<client.port<<" success delivery"<<endl;
							//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
							end = std::chrono::high_resolution_clock::now();
							elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - display_time);
							elapsed_min=elapsed.count()/60;
							elapsed_sec=elapsed.count()%60;

							fout<<elapsed_min<<":"<<elapsed_sec<<" post "<<post_msg<<" was delivered to all successfully  "<<endl;
							//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
							clock_t c_time = clock(); 
							auto read_time = std::chrono::duration_cast<std::chrono::seconds>(end - client.begin).count();
							//double read_time=double(c_time - client.begin)/CLOCKS_PER_SEC;
							if(client.token==1)
							{
							//cout << "success: read_time " << read_time << " msg time: " <<  client.bookmark << " num msg " << client.num_messages << endl;
							//cout << "read_time " << read_time << " msg time: " <<  client.message_time[client.bookmark] << endl;

								if((client.num_messages != client.bookmark -1) && read_time>client.message_time[client.bookmark])
								{
									string post_msg="post1::"+to_string(client.port)+"::"+client.msg[client.bookmark];
									cout<<client.port<<" posting "<<client.msg[client.bookmark]<<endl;
									client.sender(post_msg, client.next_id);
									//client.track_time = clock();
									client.track_time = std::chrono::high_resolution_clock::now();

									//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
									end = std::chrono::high_resolution_clock::now();
									elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - display_time);
									elapsed_min=elapsed.count()/60;
									elapsed_sec=elapsed.count()%60;

									fout<<elapsed_min<<":"<<elapsed_sec<<" post "<<post_msg<<" was sent "<<endl;
									//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
									if(client.num_messages != client.bookmark -1)
									{
										//client.state=5;
									}
									
										client.bookmark++;
								}
								else
								{
									
									client.send_token(client.next_id);
									
									if (client.token_sent == 0)
									{
										//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
										end = std::chrono::high_resolution_clock::now();
										elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - display_time);
										elapsed_min=elapsed.count()/60;
										elapsed_sec=elapsed.count()%60;

										fout<<elapsed_min<<":"<<elapsed_sec<<" token "<<client.token_id<<" was sent to client "<<client.next_id<<endl;
										client.token_sent = 1;
									}
									//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
								}
							}
						}
					}


				}//*****************************end of state 4*****************************
				if(client.state==5)
				{
					cout<<client.port<<" says BYEEE"<<endl;
					break;
				}
				
			}//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& end of isset&&&&&&&&&&&&&&&&&&&&&&&&&&
			else
			{
				auto current_time = std::chrono::high_resolution_clock::now();
				auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - client.begin).count();
				auto rtr_time = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - client.track_time).count();
										
				/*clock_t current_time = clock(); 
				double elapsed_time=(double)(current_time - client.begin)/CLOCKS_PER_SEC;
				double rtr_time=(double)(current_time - client.track_time)/CLOCKS_PER_SEC;*/

				cout<<"elapsed_time "<<elapsed_time << " leave: " << client.leave_time<<endl;

				/*if(elapsed_time>client.leave_time*1000)
				{
					client.state=5;
					continue;
				}*/
				if(client.state==1)
				{
					if (client.port == 3454)
						cout << "rtr_time: " << rtr_time << endl;
					if(rtr_time>100)
					{
						if(client.next_probe==client.lower_range)
						{
							client.next_probe=client.upper_range-1;
						}
						else if(client.next_probe==client.port-1)
						{
							cout<<client.port<<" No one in the ring"<<endl;
						}
						
						client.next_probe++;
						client.send_probe_request(client.next_probe);
						client.track_time = std::chrono::high_resolution_clock::now();
						// client.track_time = clock();

						cout<<client.port<<" sent probe to "<<client.next_probe<<endl;

					}
				}

				if(client.state==2 || client.state==3 || client.state==4)
				{
					if(rtr_time>2000)
					{
						cout<<client.port<<" Ring broke"<<endl;
						client.init();
					}
					
				}

			}
			
		}
		//l++;

	}//-----------------------------------------------------end of while----------
}