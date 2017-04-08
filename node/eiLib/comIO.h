#ifndef COMIO_H
#define COMIO_H


class ComIO
{
public:
    int comReadBufferLen;
    ComIO();
    virtual int open()=0;
    virtual void close()=0;
    virtual int read(unsigned char * buffer, int size)=0;
    virtual int write( unsigned char * msgBuffer, const int len)=0;
    static  void sleep(int ms);
};

#endif // COMIO_H
