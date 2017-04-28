#ifndef ARDUINO
#include <memory.h>
#include <iostream>
#else
#include <stdio.h>
#include "Arduino.h"
#endif


#include "eiCom.h"
#include "eimsg.h"

#include "comIOcp.h"
#include "rs232.h"

#include "msgdef.h"

using namespace eiCom;

namespace eiMsg
{


#ifndef min
#define min(a,b) ((a<b)? a : b)
#endif




char msgtest[10230]; // for testing
void EiCom::sendMsg(EiMsg & msg)
{
    io->write(msg.msg(), (int)msg.len());
   // memcpy(msgtest, msg.msg(), msg.len());
  //  msgState = mrs_readSTX;
   // seqID=-1;
   // lastseqID=-1;
   // msgID[MSGIDLEN] = 0;
   // msgIdx=0;
}




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



EiMsg::EiMsg()
{
   memset(_msgBuffer, '_', MAXMSGLEN);
   eiMsgID=72;
}

unsigned char * EiMsg::body()
{
    return &_msgBuffer[_hdrlen];
}
unsigned char *  EiMsg::msgID()
{
   return  _id;
}
int16_t EiMsg::len(void)
{
    return _msglen;
}

unsigned char EiMsg::sequenceIDin = 0;
unsigned char EiMsg::sequenceIDout = 0;

unsigned char EiMsg::getSequenceID()
{
  if(sequenceIDout>= 10)
  {
    sequenceIDout = 1;
  }
  else
  {
    sequenceIDout++;
  }
  return sequenceIDout;
}

long EiMsg::setBody(const unsigned char *msgId, MsgBody * msgbody)
{
    unsigned char buffer[MAXMSGLEN];
    unsigned char * ptr = msgbody->serialize(buffer);
    long len = ptr - buffer;
    return setBody(msgId, buffer, len);
}


long EiMsg::setBody(const unsigned char * msgId, const void * body, long len)
{
    char * secid ="1234";
    static unsigned char numUCharsInHeader = 4;
    // serialization adds 1 uchar for each string.
    static unsigned char numSmallStringsInHeader = 2;
    static unsigned char sizeofint16_t = 2;
    _hdrlen = sizeofint16_t + numUCharsInHeader + strlen(secid) + strlen((const char *)msgId) + numSmallStringsInHeader;
    unsigned char *msg = _msgBuffer;
    msg = serUChar(msg, STX);
    msg = serUChar(msg, eiMsgID);
    msg = serUChar(msg, _hdrlen);
    msg = serSmallString(msg, (char *)msgId);
    msg = serUChar(msg, getSequenceID());
    msg = serSmallString(msg, secid);
    //msg body starts  2 bytes (size of int16_t) as serChar adds size in before bodyh
    msg = serCharArr(msg, (char *) body, len);
    _msglen = _hdrlen + len;
    return _msglen;
}

void EiMsg::dump()
{
    printf("eiMsg ID %s, len %ld \n", msgID(), len());
}

MsgSeqNumAction EiCom::msgSeqAction(char * msgID)
{

    switch(atoi(msgID))
    {
        case 1: // logon
        case 42:
            return msna_ignore;
        case 2: // logged on , succes
            return msna_set;
        case 3:
            return msna_check_error;
        case 4:
            return msna_check_warn;
    default: // generic msg publication
            return msna_ignore;
    }

}


int EiCom::processMessages()
{
    char msgType;
    char hdrlen=0;
    int readBufferIndex = 0;
    int n =1;
    int numProcessed =0;
    int ignore = 0;
    unsigned char * msg;
    unsigned char secid[32];

    while(n>0)
    {
      int n = io->read(readBuffer, io->comReadBufferLen);
      if(n <= 0)
      {
          return numProcessed;
      }
      else
      {
        for(readBufferIndex = 0; readBufferIndex < n; readBufferIndex++)
        {
            numProcessed++;
            switch(msgState){
            case mrs_readSTX:
                if(readBuffer[readBufferIndex]== STX)
                {
                    msgIdx=0;
                    MSG[msgIdx++]= readBuffer[readBufferIndex];
                    msgState = mrs_readType;
                    ignore = 0;
                }
                else
                {
                    ignore++;
                }
                break;
            case mrs_readType:
                if(readBuffer[readBufferIndex] == 72) // check if supported msg type
                {
                    msgType = MSG[msgIdx++] = readBuffer[readBufferIndex];
                    msgState = mrs_ReadHdrLen;
                }
                else
                {
                    printf("invalid msg [%c] [%d]\n", readBuffer[readBufferIndex],readBuffer[readBufferIndex]);
                    // not a supported message so drop out
                    msgState = mrs_readSTX;
                    msgIdx = -1;
                }
                break;
            case mrs_ReadHdrLen:
                hdrlen= readBuffer[readBufferIndex];
                MSG[msgIdx++]=readBuffer[readBufferIndex];
                counter = hdrlen -msgIdx; //allow for STX/ msgType etc already read
                msgState = mrs_ReadHdr;
                break;
            case mrs_ReadHdr:
                MSG[msgIdx++]=readBuffer[readBufferIndex];
                if(--counter <= 0)
                {
                    msg = &MSG[MSGHDRLENOFFSET +1];
                    msg = deserSmallString(msg, (char *)msgID);
                    msg = deserUChar(msg, &seqID);
                    msg = deserSmallString(msg, (char *)secid);
                    msg = deserInt16(msg, &counter);
                    msgBodyLen = counter;
                    MsgSeqNumAction msna = msgSeqAction(msgID);
                    switch(msna)
                    {
                    case msna_check_warn:
                    case msna_check_error:
                        if(lastseqID !=  -1) //not first msg
                        {
                            if(lastseqID == MAXSEQNUM)
                            {

                                if(seqID != MINSEQNUM)
                                {
                                    printf("invalid seq number %d %d\n", seqID, lastseqID);
                                    if(msna == msna_check_error)
                                    {
                                      msgState = mrs_readSTX;
                                      msgIdx=-1;
                                    }
                                }
                            }
                            else if (lastseqID != seqID - 1)
                            {
                                 printf("invalid seq number %d %d\n", seqID, lastseqID);
                                 if(msna == msna_check_error)
                                 {
                                    msgState = mrs_readSTX;
                                    msgIdx=-1;
                                 }
                            }
                        }

                    }
                    lastseqID = seqID;
                    msgState = mrs_ReadBody;
                }
                break;
            case mrs_ReadBody:
                 MSG[msgIdx++]=readBuffer[readBufferIndex];

                if(--counter <=0)
                {
                    MSG[msgIdx]= 0;
                    msgQueue.Enqueue(msgID, &MSG[hdrlen], msgBodyLen);
                    msgState = mrs_readSTX;
                    msgIdx = -1;
                 }
                 break;
            }
        }
      }

    }
    // setup buffer for next read if chars read in msg
    return readBufferIndex;
}














int EiCom::init()
{
    return io->open();
}
void EiCom::shutdown()
{
    io->close();
}
} // eiMsg
