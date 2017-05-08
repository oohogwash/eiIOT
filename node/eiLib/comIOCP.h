#ifndef COMIOCP_H
#define COMIOCP_H
#ifdef NOT_ARDUINO

#include "comIO.h"

namespace eiMsg
{
//this class is only used for testing
//will overwrite if reads are too slow
class ReadWriteBuffer
{
    const static int bufflen = 10240;
    unsigned char *buffer;
    unsigned char * writeptr;
    unsigned char * readptr;
public:
    ReadWriteBuffer();
    ~ReadWriteBuffer();
    int read(unsigned char * buffer, int size);
    int write( const unsigned char * msgBuffer, const int len);

};



//this class is only used for testing
class ComIOsm : public ComIO
{
    int comport;
    int baudrate;
    char mode[5];
 //   unsigned char buffer[10240];
 //   unsigned char * writeptr;
 //   unsigned char * readptr;
    ReadWriteBuffer * readStream, * writeStream;
public:
    ComIOsm();
    int open();
    void close();
    int read(unsigned char * buffer, int size);
    int write( const unsigned char * msgBuffer, const int len);
    static void sleep(int ms);

    void configureStreams(ReadWriteBuffer * readStream, ReadWriteBuffer *writeStream)
    { this->readStream = readStream, this->writeStream = writeStream;}
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
