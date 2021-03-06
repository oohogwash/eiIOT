#ifdef NOT_ARDUINO

#include "osheader.h"

#include "comIOcp.h"
#include "rs232.h"
#include "eiCom.h"



namespace eiMsg
{


ReadWriteBuffer::ReadWriteBuffer()
{
    buffer = new unsigned char [bufflen];
    readptr = writeptr = buffer;
    memset(buffer, 0, bufflen);

}

ReadWriteBuffer::~ReadWriteBuffer()
{
    delete(buffer);
}

ComIOsm::ComIOsm()
{
    comReadBufferLen = 2048;
    close();

}
int ReadWriteBuffer::read(unsigned char * readBuffer, int size)
{
    if(readptr + size >= writeptr)
    {
        if(readptr > writeptr)
        {
            readptr = writeptr;
        }
        size = writeptr - readptr;
    }
    memcpy(readBuffer, readptr, size);
    readptr += size;
    return size;
}


int ReadWriteBuffer::write(const unsigned char * msgBuffer, int len)
{
    if(writeptr + len > buffer + bufflen)
    {
        memset(writeptr , 0, bufflen - (writeptr - buffer));
        writeptr=buffer;
    }
    memcpy(writeptr, msgBuffer, len);
    writeptr += len;
    return len;
}



int ComIOsm::open()
{
  //  readptr = writeptr = buffer;
    return 0;
}

void ComIOsm::close()
{
  //  readptr = writeptr = 0;
}
int ComIOsm::read(unsigned char * readBuffer, int size)
{
   /* if(readptr + size >= writeptr)
    {
        if(readptr > writeptr)
        {
            readptr = writeptr;
        }
        size = writeptr - readptr;
    }
    memcpy(readBuffer, readptr, size);
    readptr += size;
    return size;
    */
    return readStream->read(readBuffer, size);
}


int ComIOsm::write(const unsigned char * msgBuffer, int len)
{
    /*memcpy(writeptr, msgBuffer, len);
    writeptr += len;
    return len;
    */
    return writeStream->write(msgBuffer, len);
}

void ComIOsm::sleep(int ms)
{

#ifdef _WIN32
    Sleep(ms);
#else
    usleep(1000 * ms);  /* sleep for 1 Second */
#endif


}







ComIOCP::ComIOCP()
{
    comReadBufferLen = 2048;
    comport = 2;
    baudrate = 9600;
    strcpy(mode, "8N1");

}
int ComIOCP::open()
{
    return RS232_OpenComport(comport, baudrate, mode);
}

void ComIOCP::close()
{
    RS232_CloseComport(comport);
}

int ComIOCP::read(unsigned char * buffer, int size)
{
    return RS232_PollComport(comport, buffer, size);
}


int ComIOCP::write(const unsigned char * msgBuffer, int len)
{
    return RS232_SendBuf(comport, msgBuffer, len);
}
void ComIOCP::sleep(int ms)
{

#ifdef _WIN32
    Sleep(ms);
#else
    usleep(1000 * ms);  /* sleep for 1 Second */
#endif


}

} // eiMsg

#endif // NOT_ARDUINO
