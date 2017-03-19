#include "eiCom.h"
#ifndef ARDUINO
#include <memory.h>
#include <iostream>
#else
#include <stdio.h>
#include "Arduino.h"
#endif

using namespace  std;

char msgtest[10230]; // for testing
void eiCom::sendMsg(eiMsg & msg)
{
    memcpy(msgtest, msg.msg(), msg.len());
}

char eiMsg::sequenceIDin = 0;
char eiMsg::sequenceIDout = 0;

int testIdx = 0;
char readStrmChar()
{
    if(testIdx > MAXMSGLEN)
    {
        testIdx = 0;
    }
    return msgtest[testIdx++];
}
int readStrmString(char buffer [], int cnt)
{
    int idx = 0;
    while(idx < cnt)
    {
        if(testIdx > MAXMSGLEN)
        {
            testIdx = 0;
        }
        buffer[idx++] = msgtest[testIdx++];
    }
    return idx;
}

long readStrmLong(char * strm, int len = 4)
{
   char buffer[1024];
   buffer[len]=0;
   if(strm == 0)
   {
       readStrmString(buffer, len);
   }
   else
   {
       strncpy(buffer, strm, len);
   }
   long val;
   sscanf(buffer,"%4d", &val);
   return val;

}

eiMsg eiCom::readMsg()
{
    eiMsg msg;
    char buffer[MAXMSGLEN];
    MSGHEADER msgHeader;
    char ch;
    int cnt = 0;
    while(ch = readStrmChar() ){

        if(ch == STX)
        {
            readStrmString(msgHeader.eiMsgCode, 2);
            if( !strncmp(eiMsgID, msgHeader.eiMsgCode, 2) )
            {
                buffer[0]=STX;
                strncpy(&buffer[1], eiMsgID, 2);
                break; // it is an eiMsg
            }
        }

        if(cnt >MAXMSGLEN)
        {
            return msg; // no messages that are identifiable.
        }

    }

    buffer[MSGHDRLENOFFSET] = readStrmChar();
    msgHeader.hdrlen = (int)buffer[MSGHDRLENOFFSET];




    readStrmString(&buffer[MSGHDRLENOFFSET + 1], msgHeader.hdrlen - MSGHDRLENOFFSET -1);

    strncpy(msgHeader.msgID , &buffer[MSGIDOFFSET], MSGIDLEN);

    msgHeader.msgLen = readStrmLong(&buffer[MSGLENOFFSET]);

    strncpy(msgHeader.secid, &buffer[MSGSECIDOFFSET], MSGSECIDLEN);

       //read input stream until STX recieved.
    char msgBody[MAXMSGLEN];

    readStrmString(msgBody, msgHeader.msgLen);


//   msg.setID(msgHeader.msgID);
  msg.setBody(msgHeader.msgID, msgBody, msgHeader.msgLen );
  msg.setLen(msgHeader.msgLen);

}

eiMsg::eiMsg()
{
   memset(_msgBuffer, '_', MAXMSGLEN);
}

char * eiMsg::body()
{
    return &_msgBuffer[MSGBODYOFFSET];
}
char *  eiMsg::msgID()
{
   return  _id;
}
long eiMsg::len(void)
{
    char buffer[MSGLENLEN +1];
    buffer[MSGLENLEN] = 0;
    memcpy(buffer, &_msgBuffer[MSGLENOFFSET],  MSGLENLEN);
    sscanf(buffer, "%4ld", &_len);
    return _len;
}

char eiMsg::getSequenceID()
{
  if(sequenceIDout>=125)
  {
    sequenceIDout = 1;
  }
  else
  {
    sequenceIDout++;
  }
  return sequenceIDout;
}

long eiMsg::setBody(const char * msgId, const void * body, long len)
{

  if(len > MAXBODYLEN)
  {
      return MSGBODYTOLONG;
  }
  memset(_msgBuffer, ' ', len + MSGHDRLEN);
 _msgBuffer[len + MSGHDRLEN+2] = 0; // add a null terminator to allow printing as string
  strncpy(&_msgBuffer[MSGIDOFFSET], msgId, min(MSGIDLEN,strlen(msgId)));
  memcpy(&_msgBuffer[MSGBODYOFFSET], (char *)body, len);
  _msgBuffer[MSGSTXOFFSET] =STX;
//  _msgBuffer[len + MSGHDRLEN] = ETX;
  _msgBuffer[MSGSEQIDOFFSET] = getSequenceID();
  char ch =MSGBODYOFFSET;// MSGHDRLEN;
   _msgBuffer[MSGHDRLENOFFSET] = ch;
   strncpy(&_msgBuffer[MSGSECIDOFFSET], "123456789",MSGSECIDLEN);
   _msgBuffer[MSGCODEOFFSET]= eiMsgID;
   setLen( len);
    setID(msgId);
    return len + MSGHDRLEN ;
}
void eiMsg::setLen(const long msgLen)
{
    char buffer[25];
    sprintf(buffer, "%4d", msgLen);
    memcpy(&_msgBuffer[MSGLENOFFSET], buffer, MSGLENLEN);
}
void eiMsg::setID(const char * id)
{
    strncpy(&_msgBuffer[MSGIDOFFSET], id, min(MSGIDLEN,strlen(id)));
    strncpy(_id, id, MSGIDLEN);
    _id[MSGIDLEN] = '\0';
}
void eiMsg::dump()
{
    printf("eiMsg ID %s, len %ld \n", msgID(), len());
}
