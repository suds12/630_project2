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
#include "node.cpp"
#define MAX_MESSAGE_LEN 65536

using namespace std;

int main()
{
  c[1].uid=1;
  c[1].accumulate();
  c[2].uid=2;
  c[2].accumulate();
  c[1].send_msg();

}
