#ifndef MSGRECORD_H
#define MSGRECORD_H

#include <string.h>
#include "eimsg.h"

class msgRecord
{
public:
    char msgid[MSGIDLEN+1];
    int msglen;
    char msgbuffer[MAXMSGLEN];
    void update(const char * id, const int len, const char * buffer)
    {
        strncpy(msgbuffer, buffer, len);
        msgbuffer[len]=0;
        strncpy(msgid, id, MSGIDLEN);
        msgid[MSGIDLEN] = 0;
        msglen = len;
    }

     char * getBuffer()
    {
        return msgbuffer;
    }
};




#endif // MSGRECORD_H
