#include "Client.h"


Client::Client()
{
	
}
Client::~Client()
{

}	

int Client::Create_Socket(int domain, int type, int protocol)
{
	std::cout<<"Creating Socket for Client..."<<std::endl;
	m_socket=socket(domain, type, protocol);

	if(m_socket<0)
	{
		std::cout<<"Fail to create socket!"<<std::endl; 
        	
	}
	else
	{
		std::cout<<"Success to create socket!"<<std::endl; 

	}
	return m_socket;
}

void Client::Connect(short int sin_family,std::string & hostname, int port)
{
	m_server=gethostbyname(hostname.c_str());
	m_server_addr.sin_family = sin_family; 
    m_server_addr.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr*)*m_server->h_addr_list));
    m_server_addr.sin_port = htons(port);
	int status = connect(m_socket, (sockaddr*) &m_server_addr, sizeof(m_server_addr));

}

int Client::Send_Msg(){ return 0;}

int Client::Recieve_Msg(){ return 0;}







