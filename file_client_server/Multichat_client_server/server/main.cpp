#include "Server.h"
#include <thread>
#include <iostream>
using namespace std;


int main(int argc, char const *argv[])
{
     char ip_addr[100]="localhost";
    int port=8081;
    Network_Interface* network =new Server(port);
    int byte_rcv=0;
    auto server=dynamic_cast<Server*>(network);
    server->Create_Socket(AF_INET, SOCK_STREAM, 0);
    server->Bind(AF_INET, ip_addr);
    server->Listen(10);
    server->Exec();

}

