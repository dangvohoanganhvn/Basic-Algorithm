#include "Server.h"
#include <cstdlib>

Server::Server(int & port)
    :m_port(port)
{
    m_msg_send=new Message();
    m_msg_rcv=new Message();
}
Server::~Server()
{
    close(m_socket);
    close(m_client_socket);
    delete m_msg_send;
    delete m_msg_rcv;
}	

int Server::Create_Socket(int domain, int type, int protocol)
{
    int opt=1;
    std::cout<<"Creating Socket for Server..."<<std::endl;
    m_socket=socket(domain, type, protocol);

    if(m_socket<0)
    {
        perror("Create_Socket:");
        std::cout<<"Fail to create socket!"<<std::endl;
        exit(1);

    }
    else
    {
        if (setsockopt(m_socket, SOL_SOCKET, SO_REUSEPORT,
                       &opt, sizeof(opt)))
        {
            perror("Setsockopt:");
            exit(EXIT_FAILURE);
        }
        std::cout<<"Success to create socket!"<<std::endl;
    }
    fdmax=m_socket;
    FD_ZERO(&master);    // clear the master and temp sets
    FD_ZERO(&read_fds);
    FD_SET(m_socket, &master);
    return m_socket;
}

// Send a message to a socket
int Server::Send_Msg(int socket)
{
    memset(&buffer_send, 0, sizeof(buffer_send)); //clear buffer
    this->Encapsulate(m_msg_send,buffer_send);
    int total = 0;        // how many bytes we've sent
    int bytesleft = m_msg_send->m_len; // how many we have left to send
    int n;

    while(total < m_msg_send->m_len) {
        n = send(socket, buffer_send+total, bytesleft, 0);
        if (n == -1) { break; }
        total += n;
        bytesleft -= n;
    }

    return n==-1?-1:0; //

}

int Server::Recieve_Msg(int socket){
    memset(&buffer_rcv, 0, sizeof(buffer_rcv)); //clear buffer

    int num_rcv=recv(socket, buffer_rcv  , sizeof(buffer_rcv), 0);
    this->Parse(m_msg_rcv,buffer_rcv);
    if(m_msg_rcv->m_type==CHAT)
    {
        std::cout << "Client " << m_msg_rcv->m_src_id<<" to Client "<<m_msg_rcv->m_des_id<<": "<<m_msg_rcv->data<< std::endl;
    }

    return num_rcv;
}

int Server::Bind(short int sin_family, char* s_addr)
{
    /* setup the host_addr structure for use in bind call */
    // server byte order

    hp=gethostbyname(s_addr); //get info host name
    if (hp == NULL) {
        std::cout<<"Cannot resolve the host name: "<<s_addr<<std::endl;
        perror("Gethostbyname:");
        exit(1);
    }
    m_server_addr.sin_family = sin_family;

    // Allocate IP address
    m_server_addr.sin_addr =*((struct in_addr *)hp->h_addr);

    // convert short integer value for port must be converted into network byte order
    m_server_addr.sin_port = htons(m_port);

    //Bind to socket
    auto ret=bind(m_socket, (struct sockaddr *) &m_server_addr,sizeof(m_server_addr)) ;

    if(ret==-1)
    {
        std::cout<<"Fail to Bind to the socket at Port "<<m_port<<std::endl;
        close(m_socket);
        exit(1);
    }
    else
    {
        std::cout<<"Success to Bind to the socket at Port "<<m_port <<std::endl;
    }

    return ret;
}

void Server::Listen(int backlog)
{
    listen(m_socket,backlog);
    std::cout<<"Linstenting at port "<<m_port <<std::endl;
}

void Server::Accept()
{
    m_client_socket = accept(m_socket, (struct sockaddr *) &m_client_addr, &clilen);
    if(m_client_socket==-1)
    {
        std::cout<<"Fail to get connection" <<std::endl;
        exit(1);
    }
    else
    {
        std::cout<<"Success to get connection from " <<inet_ntoa(m_client_addr.sin_addr)<<" at port "<<ntohs(m_client_addr.sin_port)<< std::endl;
        FD_SET(m_client_socket, &master); // add to master set
        if (m_client_socket > fdmax) {    // keep track of the max
            fdmax = m_client_socket;
        }
    }
}
int Server::GetClientSocket() const
{
    return m_client_socket ;
}

void Server::Exec()
{
    timeval Timeout; // timer is used for non blocking socket
    Timeout.tv_sec = 1;
    Timeout.tv_usec = 0;
    while(1)
    {
        read_fds = master; // copy it
        if (select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1)
        {
            perror("select");
            exit(4);
        }
        // run through the existing connections looking for data to read
        for(int i = 0; i <= fdmax; i++) {
            if (FD_ISSET(i, &read_fds)) { // we got one!!
                if (i == m_socket)
                {
                    // handle new connections
                    clilen = sizeof remoteaddr;
                    this->Accept();

                    if (m_client_socket == -1)
                    {
                        perror("accept");
                    }
                    else
                    {
                        do
                        {
                            m_id_max_client=rand() % 1000 + 1;
                        } while (m_list_clients.find(m_id_max_client)!=m_list_clients.end()); //generate id for new client

                        m_list_clients.insert(std::pair<int,int>(m_id_max_client,m_client_socket));
                        std::cout<<"Add new client with id "<<m_id_max_client<<std::endl;

                        FD_SET(m_client_socket, &master); // add to master set
                        if (m_client_socket > fdmax)
                        {    // keep track of the max
                            fdmax = m_client_socket;
                        }
                        std::string id = std::to_string(m_id_max_client);
                        SetDataSend(id,AUTHEN,0,0);
                        Send_Msg(m_client_socket);
                        //Noti to other Client

                        std::string noti_online="Client "+std::to_string(m_id_max_client)+" is online";
                        SetDataSend(noti_online,CHAT,0,0);
                        for(int j = 0; j <= fdmax; j++)
                        {
                            // send to all!
                            if (FD_ISSET(j, &master))
                            {

                                // except the listener and ourselves
                                if (j != m_socket && j != i)
                                {
                                    this->Send_Msg(j);
                                }
                            }
                        }



                    }
                }
                else
                {
                    // handle data from a client
                    byte_rcv= this->Recieve_Msg(i);
                    if (byte_rcv<= 0) {
                        // got error or connection closed by client
                        if (byte_rcv == 0)
                        {
                            // connection closed
                            printf("Server: socket %d hung up\n", i);
                        }
                        else
                        {
                            perror("recv");
                        }

                        close(i); // bye!
                        FD_CLR(i, &master); // remove from master set
                        std::map<int,int>::iterator it;
                        for (it = m_list_clients.begin(); it!= m_list_clients.end(); ++it)
                            if(it->second==i){
                                std::cout<<"Remove "<<it->first<<" From Client list"<<std::endl;

                                break;
                            }
                        m_list_clients.erase(it);
                    }
                    else {
                        if(m_msg_rcv->data.compare("quit")==0){

                            std::cout<<"Close socket: "<<i<<std::endl;
                            FD_CLR(i, &master); // remove from master set.
                            std::map<int,int>::iterator it;
                            for (it = m_list_clients.begin(); it!= m_list_clients.end(); ++it)
                                if(it->second==i){
                                    std::cout<<"Remove "<<it->first<<" From Client list"<<std::endl;

                                    break;
                                }
                            m_list_clients.erase(it);
                            close(i);
                            continue;
                        }
                        if(m_msg_rcv->m_type==CHAT)
                        {
                            if(m_list_clients.find(m_msg_rcv->m_des_id)!=m_list_clients.end()){
                                SetDataSend(m_msg_rcv->data,CHAT,m_msg_rcv->m_src_id,m_msg_rcv->m_des_id);
                            }
                            else {
                                std::string  noti="The client you want to send is offline";
                                std::cout<<"Client "<<m_msg_rcv->m_des_id<<" is not in client list"<<std::endl;
                                SetDataSend(noti,CHAT,0,0);
                                Send_Msg(i); //Send noti to client
                                break;
                            }


                            // we got some data from a client
                            for(int j = 0; j <= fdmax; j++)
                            {
                                // send to des!
                                if (FD_ISSET(j, &master))
                                {

                                    if (j != m_socket && j != i&&j==m_list_clients[m_msg_send->m_des_id])
                                    {
                                        this->Send_Msg(j);
                                    }
                                }
                            }
                        }//END for send msg CHAT to client
                    }
                } // END handle data from client
            } // END got new incoming connection
        } // END looping through file descriptors
    } // END for while(1)--and you thought it would never end!
}

void Server::SetDataSend(std::string& data, unsigned char type, uint16_t src_id, uint16_t des_id)
{
    m_msg_send->data=data;
    m_msg_send->m_type=type;
    m_msg_send->m_src_id=src_id;
    m_msg_send->m_des_id=des_id;
    m_msg_send->m_len=data.length()+9;
}



