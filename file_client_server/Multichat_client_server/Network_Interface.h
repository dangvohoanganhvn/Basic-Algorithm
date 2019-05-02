#ifndef __NETWORK_INTERFACE_H__
#define __NETWORK_INTERFACE_H__

#include <iostream>
#include <string>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <fstream>

#define TIMER_OUT 90
#define SERVER_ID 0

class Network_Interface
{
public:

	/*!
		* \brief Proper destruction of base class references.
		*/
	virtual ~Network_Interface(){};	
	/**
		* \brief Attempts to Create a Socket.
		*/
	virtual int Create_Socket(int domain, int type, int protocol)=0;

	/**
		* \brief Attempts to Send a message.
		*/
    virtual int Send_Msg(int socket)=0;
	/**
		* \brief Attempts to Recieve a message.
		*/
    virtual int Recieve_Msg(int socket)=0;

};
#endif
