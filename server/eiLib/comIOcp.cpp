#include "comIOcp.h"
#include "rs232.h"
#include "eiCom.h"

comIOCP::comIOCP()
{
    comReadBufferLen = 2048;
    comport = 2;
    baudrate = 9600;
    strcpy(mode, "8N1");

}
int comIOCP::open()
{
    return RS232_OpenComport(comport, baudrate, mode);
}

void comIOCP::close()
{
    RS232_CloseComport(comport);
}

int comIOCP::read(unsigned char * buffer, int size)
{
    return RS232_PollComport(comport, buffer, size);
}


int comIOCP::write(unsigned char * msgBuffer, int len)
{
    return RS232_SendBuf(comport, msgBuffer, len);
}
void comIOCP::sleep(int ms)
{
#ifdef _WIN32
    Sleep(ms);
#else
    usleep(1000 * ms);  /* sleep for 1 Second */
#endif

}


