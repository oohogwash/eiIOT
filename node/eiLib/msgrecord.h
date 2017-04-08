#ifndef MSGRECORD_H
#define MSGRECORD_H
#include <memory.h>

#include "eimsg.h"

//#include <memory.h>

namespace eiMsg
{

//using namespace ei;


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
