#ifndef COMIOCP_H
#define COMIOCP_H
#include "comIO.h"

class comIOarduino : public comIO
{
    int comport;
    int baudrate;
    int mode;
public:
    comIOarduino();
    int open();
    void close();
    int read(unsigned char * buffer, int size);
    int write( unsigned char * msgBuffer, const int len);
    static void sleep(int ms);
};

#endif // COMIOCP_H
