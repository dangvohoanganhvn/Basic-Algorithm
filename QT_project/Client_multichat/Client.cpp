#include "Client.h"


Client::Client()
{
    m_msg_send=new Message();
    m_msg_rcv=new Message();
}
Client::~Client()
{
    close(m_socket);
    delete m_msg_send;
    delete m_msg_rcv;
}	

int Client::Create_Socket(int domain, int type, int protocol)
{
    std::cout<<"Creating Socket for Client..."<<std::endl;
    m_socket=socket(domain, type, protocol);

    fcntl(m_socket, F_SETFL, O_NONBLOCK);
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

void Client::Connect(short int sin_family,std::string & hostname, int& port)
{
    timeval Timeout; // timer is used for non blocking socket
    Timeout.tv_sec = TIMER_OUT;
    Timeout.tv_usec = 0;
    fd_set write;
    fcntl(m_socket, F_SETFL, O_NONBLOCK);

    std::cout<<"Connecting to server!"<<std::endl;
    bzero((char*)&m_server_addr, sizeof(m_server_addr));
    m_server=gethostbyname(hostname.c_str());
    if (m_server == NULL) {
        std::cout<<"Cannot resolve the server name: "<<m_server<<std::endl;
        perror("Gethostbyname:");
        exit(1);
    }

    m_server_addr.sin_family = sin_family;
    m_server_addr.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr*)*m_server->h_addr_list));;
    m_server_addr.sin_port = htons(port);

    int status = connect(m_socket, (sockaddr*) &m_server_addr, sizeof(m_server_addr));
    if(status<0)
    {
        if(errno == EINPROGRESS ){
            //fprintf(stderr, "EINPROGRESS in connect() - selecting\n");
            do {

                FD_ZERO(&write);
                FD_SET(m_socket, &write);
                status = select(m_socket+1, nullptr, &write, nullptr, &Timeout);
                if (status < 0 && errno != EINTR) {
                    fprintf(stderr, "Error connecting %d - %s\n", errno, strerror(errno));
                    exit(1);
                }
                else if (status > 0) {
                    break;
                }
                else {
                    fprintf(stderr, "Timeout in select() - Cancelling!\n");
                    exit(1);
                }
            } while (1);
        }
    }
    std::cout<<"Success to connect to server!"<<std::endl;
}

/*
 * // have to call SetData before call this function
 * // This will Send data in buffer to the socket
 * */
int Client::Send_Msg(int socket){
    memset(&buffer_send, 0, sizeof(buffer_send)); //clear buffer

    this->Encapsulate(m_msg_send,std::ref(buffer_send));

    auto tmp= send(socket,buffer_send, m_msg_send->m_len, 0);

    return tmp;
}

int Client::Recieve_Msg(int socket){
    memset(&buffer_rcv, 0, sizeof(buffer_rcv)); //clear buffer
    int num_rcv=recv(socket, buffer_rcv  , sizeof(buffer_rcv), 0);



    if(num_rcv>0)
    {
        this->Parse(m_msg_rcv,buffer_rcv);
        if(m_msg_rcv->m_type==CHAT){
            if(m_msg_rcv->m_src_id==SERVER_ID)
            {
                std::cout << "Server: "<< m_msg_rcv->data<< std::endl;
            }
            else {
                std::cout << "Client " <<m_msg_rcv->m_src_id<<" :"<< m_msg_rcv->data<< std::endl;
            }
        }

    }
    if(num_rcv==0)
    {
        close(m_socket);
        std::cout<<"Close connection"<<std::endl;
    }


    return num_rcv;

}

int Client::GetMySocket() const
{
    return m_socket ;
}

void Client::SetDataSend(std::string& data, unsigned char type, uint16_t src_id, uint16_t des_id)
{
    m_msg_send->data=data;
    m_msg_send->m_type=type;
    m_msg_send->m_src_id=src_id;
    m_msg_send->m_des_id=des_id;
    m_msg_send->m_len=data.length()+9;
}
int Client::GetMySourceIdSend() const
{
    return m_msg_send->m_src_id;
}

void Client::SetSrcIdSend(uint16_t des_id)
{
    m_msg_send->m_src_id=des_id;
}

void Client::SetDesIdSend(uint16_t des_id)
{
    m_msg_send->m_des_id=des_id;
}

Message *Client::GetMsgSend() const
{
    return m_msg_send;
}

Message *Client::GetMsgRcv() const
{
    return m_msg_rcv;
}

int Client::GetMyDesIdSend() const
{
    return m_msg_send->m_des_id;
}





