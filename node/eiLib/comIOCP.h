#ifndef COMIOCP_H
#define COMIOCP_H
#ifdef NOT_ARDUINO

#include "comIO.h"

namespace eiMsg
{

//this class is only used for testing
class ComIOsm : public ComIO
{
    int comport;
    int baudrate;
    char mode[5];
    unsigned char buffer[10240];
    unsigned char * writeptr;
    unsigned char * readptr;
public:
    ComIOsm();
    int open();
    void close();
    int read(unsigned char * buffer, int size);
    int write( const unsigned char * msgBuffer, const int len);
    static void sleep(int ms);
};



class ComIOCP : public ComIO
{
    int comport;
    int baudrate;
    char mode[5];
public:
    ComIOCP();
    int open();
    void close();
    int read(unsigned char * buffer, int size);
    int write( const unsigned char * msgBuffer, const int len);
    static void sleep(int ms);
};


} // eiMsg

#endif // NOT_ARDUINO

#endif // COMIOCP_H
