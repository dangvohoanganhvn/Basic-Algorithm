#include "Client.h"
#include <thread>
#include <mutex>
#include <unistd.h>

using namespace std;

void Send(Client* client);

void Rcv(Client* client);

bool is_digit(const char value) { return std::isdigit(value); }
bool is_numeric(const std::string& value) { return std::all_of(value.begin(), value.end(), is_digit); }

int main(int argc, char const *argv[])
{
    std::string ip_addr="localhost";
    int port=8081;
    Network_Interface* network =new Client();
    auto client=dynamic_cast<Client*>(network);
    client->Create_Socket(AF_INET, SOCK_STREAM, 0);
    client->Connect(AF_INET,ip_addr,port);

    thread send_thread(Send,client);
    thread rcv_thread(Rcv,client);
    send_thread.join();
    rcv_thread.join();


    return 0;
}

void Send(Client* client)
{
   /* cout<<"Please enter your user name to retrieve messages"<<endl;
    std::string user;
    getline(std::cin, user);
    client->SetDataSend(user,LOGIN,0,0);
    client->Send_Msg(client->GetMySocket()); */
    while(1){
        std::string tmp;
        getline(std::cin, tmp); //input the src id
        if(is_numeric(tmp))
        {
            int n = std::stoi(tmp);
            client->SetDesIdSend(n);
        }
        else {
            if(tmp.compare("quit")==0)
            {
                std::string quit="quit";
                client->SetDataSend(quit,CHAT,0,0);
                client->Send_Msg(client->GetMySocket());
                break;
            }
            cout<<"Please specify the id of client you want to send"<<endl;
            continue;
        }

        cout<<"To Client "<<client->GetMyDesIdSend()<<": ";
        std::string data;
        getline(std::cin, data);

        client->SetDataSend(data,CHAT,client->GetMySourceIdSend(),client->GetMyDesIdSend());
        client->Send_Msg(client->GetMySocket());
        usleep(100); //avoid full of memory due to unblock socket
    }
}

void Rcv(Client* client)
{
    while(1){
        int num_rcv=client->Recieve_Msg(client->GetMySocket());
        if(num_rcv==0)
        {
            std::cout<<"Connection was closed"<<std::endl;
            close  (client->GetMySocket());
            break;
        }
        else if (num_rcv>0&&client->GetMsgRcv()->m_type==AUTHEN) {
            int i=std::stoi(client->GetMsgRcv()->data);
            client->SetSrcIdSend(i);
            cout<<"Your id is "<<i<<endl;
        }
        usleep(100);
    }
}
