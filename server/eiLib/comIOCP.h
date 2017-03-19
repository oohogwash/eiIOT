#ifndef COMIOCP_H
#define COMIOCP_H
#include "comIO.h"

class comIOCP : public comIO
{
    int comport;
    int baudrate;
    char mode[5];
public:
    comIOCP();
    int open();
    void close();
    int read(unsigned char * buffer, int size);
    int write( unsigned char * msgBuffer, const int len);
    static void sleep(int ms);
};

#endif // COMIOCP_H