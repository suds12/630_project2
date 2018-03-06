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
#define MAX_MESSAGE_LEN 65536

using namespace std;

void client_process::gen_token()
{
	token=1;
}

void client_process::send_post()
{
	int read_time;

}