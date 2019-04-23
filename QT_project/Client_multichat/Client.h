#ifndef __CLIENT_H__
#define __CLIENT_H__

#include "Network_Interface.h"
#include <string>
#include <mutex>
#include "message.h"
#include <vector>
#include <mutex>



class Client: public Network_Interface,public Process_Msg
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
    int Send_Msg(int socket);

	/**
		* \Implement Send a message on server side.
		*/
    int Recieve_Msg(int socket);

	
	/**
		* \Implement Connect to a socket.
		*/
    void Connect(short int sin_family,std::string & hostname, int& port);

    /**
        * \Implement Get the socket.
        */
    int GetMySocket() const;

    /**
        * \Implement Get the source id for seding packet.
        */
    int GetMySourceIdSend() const;

    /**
        * \Implement Get the destination id for seding packet.
        */
    int GetMyDesIdSend() const;

    /**
        * \Implement Set the Data for seding packet.
        * data: the content of message you want to send
        * type: type of message ( CHAT, AUTHEN,..)
        * src_id: the source id of the message
        * des_id: the des id that message need to be foward
        */
    void SetDataSend(std::string& data,unsigned char type, uint16_t src_id,uint16_t des_id);

    /**
        * \Implement Set the destination id for seding packet.
        */
    void SetDesIdSend(uint16_t des_id);

    /**
        * \Implement Set the source id for seding packet.
        */
    void SetSrcIdSend(uint16_t des_id);

    /**
        * \Implement Get the pointer to the message recieved from other client or Server.
        */
    Message *GetMsgSend() const;

    /**
        * \Implement Get the pointer to the message that will be sent to other client.
        */
    Message *GetMsgRcv() const;

private:
    struct sockaddr_in m_server_addr;
    struct hostent* m_server;
	int m_socket;

    //buffer for sending and recieving
    unsigned char buffer_send[256];
    unsigned char buffer_rcv[256];

/*uint32_t m_len;
    unsigned char m_type; //1 byte
    uint16_t m_id; // 2 bytes
    uint16_t  m_des_id; // 2 bytes
    std::string data;
    */
    Message* m_msg_send;
    Message* m_msg_rcv;
};
#endif
