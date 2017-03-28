#include "eiCom.h"
#ifndef ARDUINO
#include <memory.h>
#include <iostream>
#include "comIOcp.h"
#include "rs232.h"
#else
#include "comIOarduino.h"
#include <stdio.h>
#include "Arduino.h"
#endif





using namespace  std;

char msgtest[10230]; // for testing
void eiCom::sendMsg(eiMsg & msg)
{
  io->write( msg.msg(), msg.len());
  /*  memcpy(msgtest, msg.msg(), msg.len());
    msgState = mrs_readSTX;
    seqID=-1;
    lastseqID=-1;
    msgID[MSGIDLEN] = 0;
    */
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
   sscanf(buffer,"%4d", val);
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
            //if( !strncmp(eiMsgID, msgHeader.eiMsgCode, 2) )
            {
                buffer[0]=STX;
           //     strncpy(&buffer[1], eiMsgID, 2);
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
  return msg;
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
    return _len + + MSGHDRLEN;
}

char eiMsg::sequenceIDin = 0;
char eiMsg::sequenceIDout = 0;

char eiMsg::getSequenceID()
{
  if(sequenceIDout>=10)
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

int eiCom::processMessages()
{
    int idx=0;
    char msgType;
    char hdrlen=0;
    int counter = 0;
    char msgBodyBuffer[MSGLENLEN +1];
    msgBodyBuffer[MSGLENLEN]=0;
    int readBufferIndex = 0;
    int n =1;
    int numProcessed =0;


    while(n>0)
    {
      int n = io->read(readBuffer, io->comReadBufferLen);
        printf("n=%d\n",n);
      if(n <= 0)
      {
          printf("empty, leave\n");
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
                    idx=0;
                    MSG[idx++]= readBuffer[readBufferIndex];
                    msgState = mrs_readType;
                }
                else
                {
                   // printf("ignore %c\n", buf[i]);
                }
                break;
            case mrs_readType:
                if(readBuffer[readBufferIndex] == 72) // check if supported msg type
                {
                    msgType = MSG[idx++] = readBuffer[readBufferIndex];
                    msgState = mrs_ReadHdrLen;
                }
                else
                {
                    printf("invalid msg %c %d\n", readBuffer[readBufferIndex],readBuffer[readBufferIndex]);
                    // not a supported message so drop out
                    msgState = mrs_readSTX;
                }
                break;
            case mrs_ReadHdrLen:
                hdrlen= readBuffer[readBufferIndex];
                MSG[idx++]=readBuffer[readBufferIndex];
                counter = hdrlen -idx; //allow for STX/ msgType etc already read
              //  printf("hdr len %d\n", hdrlen);
                msgState = mrs_ReadHdr;
                break;
            case mrs_ReadHdr:
                MSG[idx++]=readBuffer[readBufferIndex];
                if(--counter <= 0)
                {
                    memcpy(msgBodyBuffer,&MSG[MSGLENOFFSET], MSGLENLEN);
                    msgBodyLen = counter = atoi(msgBodyBuffer);
                    memcpy(msgID, &MSG[MSGIDOFFSET], MSGIDLEN);
                    seqID = MSG[MSGSEQIDOFFSET];

                    if(lastseqID !=  -1) //not first msg
                    {
                        if(lastseqID == MAXSEQNUM)
                        {
                            if(seqID != MINSEQNUM)
                            {
                                printf("invalid seq number %d %d\n", seqID, lastseqID);
                                msgState = mrs_readSTX;
                            }
                        }
                        else if (lastseqID != seqID - 1)
                        {
                             printf("invalid seq number %d %d\n", seqID, lastseqID);
                             msgState = mrs_readSTX;
                        }
                    }
                    lastseqID = seqID;
                   // printf("sequence id = %d\n",seqID );
                  //printf("msg body len %d\n", counter);
                    msgState = mrs_ReadBody;
                 //   printf("sequence id = [%c] %d\n", MSG[MSGSEQIDOFFSET], MSG[MSGSEQIDOFFSET]);
                }
                break;
            case mrs_ReadBody:
                MSG[idx++]=readBuffer[readBufferIndex];
                if(--counter <=0)
                {
                    MSG[idx]= 0;
                  //  printf("[%s]%d\n", MSG, idx);
                    msgQueue.Enqueue(msgID, &MSG[MSGBODYOFFSET], msgBodyLen);
                   //mgr->addMessage(msgID, MSG, idx);
                   printf("msg added queue size = %d\n", msgQueue.Size());
                    msgState = mrs_readSTX;
                 }
                break;
            }
        }
      }

    }
    return readBufferIndex;
}

int eiCom::init()
{
    return io->open();
}
void eiCom::shutdown()
{
    io->close();
}
