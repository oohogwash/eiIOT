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
    char msgbuffer[MAXMSGLEN];
    void update(const char * id, const int len, const char * buffer)
    {
        memcpy(msgbuffer, buffer, len);
        msgbuffer[len]=0;
        memcpy(msgid, id, MSGIDLEN);
        msgid[MSGIDLEN] = 0;
        msglen = len;
    }

     char * getBuffer()
    {
        return msgbuffer;
    }
};

} // eiMsg




#endif // MSGRECORD_H
