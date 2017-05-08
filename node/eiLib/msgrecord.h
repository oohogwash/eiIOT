#ifndef MSGRECORD_H
#define MSGRECORD_H

#ifdef NOT_ARDUINO

#include <memory.h>
#else
# include <string.h>
#define memcpy strncpy

#endif //NOT_ARDUINO

#include "eimsg.h"

//#include <memory.h>

namespace eiMsg
{


class MsgRecord
{
public:
    char msgid[MSGIDLEN+1];
    int msglen;
    unsigned char msgbuffer[MAXMSGLEN];
    void update(const char * id, const int len, const unsigned char * buffer)
    {
        memcpy(msgbuffer, buffer, len);
        msgbuffer[len]=0;
        memcpy(msgid, id, MSGIDLEN);
        msgid[MSGIDLEN] = 0;
        msglen = len;
    }

     unsigned char ** getBuffer()
    {
        return ( unsigned char **) &msgbuffer;
    }
};

} // eiMsg




#endif // MSGRECORD_H
