#ifndef __SERVER_H__
#define __SERVER_H__

#include "Network_Interface.h"
#include <string>
#include <map>
#include "message.h"

#define MAX_CLIENT 30


class Server: public Network_Interface,public Process_Msg
{
public:
    /**
        * \Implement Constructor.
        */
    Server(int & port);

    /**
        * \Implement Destructor.
        */
    ~Server();

    /**
        * \Implement Create a Socket on server side.
        */
    int Create_Socket(int domain, int type, int protocol);

    /**
        * \Implement Send a message on server side.
        */
    int Send_Msg( int socket);

    /**
        * \Implement Send a message on server side.
        */

    int Recieve_Msg(int socket);

    /**
        * \Implement Bind to a socket.
        */
    int Bind(short int sin_family, char* s_addr);

    /**
        * \Implement Listen to a socket.
        */
    void Listen( int backlog);

    /**
        * \Implement accept client connections.
        */
    void Accept();

    /**
        * \Implement Get info client socket.
        */
    int GetClientSocket() const;

    /**
        * \Implement Set the Data for seding packet.
        * data: the content of message you want to send
        * type: type of message ( CHAT, AUTHEN,..)
        * src_id: the source id of the message
        * des_id: the des id that message need to be foward
        */
    void SetDataSend(std::string& data,unsigned char type, uint16_t src_id,uint16_t des_id);

    /**
        * \Implement Execute server side.
        */
    void Exec();

    void ThreadAccept();


private:
    struct sockaddr_in m_server_addr;
    struct sockaddr_in m_client_addr ;
    int m_socket, m_client_socket;
    int m_port; // number of port
    int byte_rcv; //number bytes recieved
    socklen_t clilen;


    struct hostent *hp; //store info hostname
    int fdmax; //file desciptor max
    fd_set master;    // master file descriptor list
    fd_set read_fds;  // temp file descriptor list for select()
    struct sockaddr_storage remoteaddr; // client address
    std::map<int,int> m_list_clients; // list of clients
    std::map<int,int> m_list_clients_socket; // list of clients

    int m_id_max_client=0;

    // Buffer for sending and recieving
    unsigned char buffer_send[50000000];
    unsigned char buffer_rcv [5000000];


    /*uint32_t m_len;
   unsigned char m_type; //1 byte
   uint16_t m_id; // 2 bytes
   uint16_t  m_des_id; // 2 bytes
   std::string data;
   */
    Message* m_msg_send;
    Message* m_msg_rcv;

    bool remain_stt=false;
    unsigned int m_last_length_rcv=0;

};
#endif
