#ifndef MESSAGE_H
#define MESSAGE_H
#include <string>


#define AUTHEN 0
#define CHAT   1
#define LOGIN   2

/**
    * \This class will define the structure of a message sent and implement for processing data..
    */

/*
 * |+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * |             |             |                |                   |       |
 * | Len(4 bytes)| Type(1 byte)|src id(2 bytes) | des id(2 bytes)   | data  |
 * |             |             |                |                   |       |
 * |+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 */
class Message{
public:
    uint32_t m_len;
    unsigned char m_type; //1 byte
    uint16_t m_src_id; // 2 bytes
    uint16_t  m_des_id; // 2 bytes
    std::string data;
};

class Process_Msg
{
public:
    ~Process_Msg();
    Process_Msg();
    /**
        * \Implement Pack the message and store it in buffer before sending to other clients.
        */
    bool Encapsulate(Message* msg, unsigned char* buffer);
    /**
        * \Implement Unpack the message and store it in buffer after receiving from other clients or server.
        */
    bool Parse(Message* msg,unsigned char* buffer);

    /**
        * \Implement Convert the content in unsigned char array[4] to a number.
        */
    void Char2Uint32(unsigned char* array, uint32_t& number);

    /**
        * \Implement Convert the content in unsigned char array[2] to a number.
        */
    void Char2Uint16(unsigned char *array, uint16_t &number);
};




#endif // MESSAGE_H
