#ifndef __CLIENT_H__
#define __CLIENT_H__

#include "Network_Interface.h"
#include <string>

class Client: public Network_Interface
{
public:
	/**
		* \Implement Constructor.
		*/
	Client();

	/**
		* \Implement Destructor.
		*/
	~Client();

    /**
		* \Implement Create a Socket on server side.
		*/
	int Create_Socket(int domain, int type, int protocol);

	/**
		* \Implement Send a message on server side.
		*/
	int Send_Msg();

	/**
		* \Implement Send a message on server side.
		*/
	int Recieve_Msg();

	
	
	/**
		* \Implement Connect to a socket.
		*/
	void Connect(short int sin_family,std::string & hostname, int port);


private:
	struct sockaddr_in m_server_addr;
	struct hostent* m_server;
	int m_socket;
	
};
#endif