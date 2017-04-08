#ifndef COMIOCP_H
#define COMIOCP_H
#include "comIO.h"
namespace eiMsg
{

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
    int write( unsigned char * msgBuffer, const int len);
    static void sleep(int ms);
};


} // eiMsg
#endif // COMIOCP_H
