#include "message.h"
#include <vector>

Process_Msg::Process_Msg()
{
}
Process_Msg::~Process_Msg()
{

}
bool Process_Msg::Encapsulate(Message *msg, unsigned char *buffer)
{
    unsigned int len = msg->data.length()+9;
    unsigned char len_byte[4]; //length field wich contain the total length of msg
    memcpy(len_byte, &len, 4);

    //copy src id
    uint16_t src_id=msg->m_src_id;
    unsigned char src_id_byte[2];
    memcpy(src_id_byte,&src_id, 2);

    //copy des id
    uint16_t des_id=msg->m_des_id;
    unsigned char des_id_byte[2];
    memcpy(des_id_byte,&des_id, 2);

    for(unsigned int i = 0; i<len; i++)
    {
        if (i<4) //copy length of total msg
        {
            *buffer=len_byte[i];
        }
        else if (i == 4) // copy type msg
        {
            *buffer=msg->m_type;
        }
        else if (i<7)
        {
            *buffer=src_id_byte[i-5];
        }
        else if(i<9)
        {
            *buffer=des_id_byte[i-7];
        }
        else
        {
            *buffer=static_cast<unsigned char>(msg->data[i-9]); //copy data
        }
        buffer++;
    }
    return true;

}

bool Process_Msg::Parse(Message *msg, unsigned char *buffer)
{

    unsigned char len_msg_c[4] = {buffer[0],buffer[1],buffer[2],buffer[3]};
    Char2Uint32(len_msg_c,msg->m_len); //Parse length



    //Parse type msg
    msg->m_type = buffer[4];

    //Parse src id
    unsigned char src_id[2] = {buffer[5],buffer[6]};
    Char2Uint16(src_id,msg->m_src_id);

    //Parse des id
    unsigned char des_id[2] = {buffer[7],buffer[8]};
    Char2Uint16(des_id,msg->m_des_id);


    std::string tmp(reinterpret_cast<char*>(buffer+9));
    msg->data=tmp;

}


void Process_Msg::Char2Uint32(unsigned char *array, uint32_t &number)
{
    number = 0;
    for (unsigned int i=0; i<4; i++)
    {
        unsigned int j = static_cast<unsigned int>(*array++) << i*8;
        number |= j;
    }

}

void Process_Msg::Char2Uint16(unsigned char *array, uint16_t &number)
{
    number = 0;
    for (unsigned int i=0; i<2; i++)
    {
        unsigned int j = static_cast<unsigned int>(*array++) << i*8;
        number |= j;
    };

}
