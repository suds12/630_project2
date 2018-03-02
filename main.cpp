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
//-------
#include "reader.cpp"
#define MAX_MESSAGE_LEN 65536

using namespace std;

int main(int argc, char** argv)
{
  client.input_file=argv[1];
  client.config_file=argv[2];
  client.input_reader();
  client.config_reader();
}
