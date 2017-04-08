#ifndef EICOM_H_
#define EICOM_H_

#include "comIO.h"
#include "eimsg.h"
#include "msgrecord.h"
#include "eiqueue.h"
#include <memory.h>
#include <stdio.h>

using namespace eiMsg;

const int EI_USER = 1000;

typedef enum msg_cmd
{
    UpdateIOPoint = EI_USER,
    UpdateDisplayWidget,
    UpdateDisplay,
    UpdateModule,
    UpdateConnection,

}MSG_CMD;

typedef enum cmd_action
{
    ca_ADD,
    ca_CHANGE,
    ca_DELETE,
    ca_OBSERVE
}CMD_ACTION;


typedef enum _state
{
	CREATED,
	RUNNING,
	PAUSED,

}STATE;


typedef enum _cmd
{
	RUN,
	PAUSE,
	RESTART,

}CMD;







const int MSGBODYTOLONG = -1;

   // STX, HDRLEN, MSGID, MSGLEN,SECID, MSGBODY,  ETX


class MsgRecordMgr;
class EiCom
{
    enum msgReadState
    {
      mrs_readSTX,
      mrs_readType,
        mrs_ReadHdrLen,
        mrs_ReadHdr,
        mrs_ReadBody

    }mrs;



    MsgRecordMgr * mgr;
    ComIO  *  io;

    int msgBodyLen;

    msgReadState msgState;
    int seqID;
    int lastseqID;
    int msgIdx;
    char msgID[MSGIDLEN+1];
    char MSG[MAXMSGLEN+1];
   static const int READBUFFER_SIZE  = 2048;
    unsigned char readBuffer[READBUFFER_SIZE+1];
    static const int MAXSEQNUM = 10;
    static const int MINSEQNUM = 1;
    int counter ;
    MsgSeqNumAction msgSeqAction(char * msgID);

public:


    EiCom( ComIO * io = 0)
    {
        this->io = io;
        msgState = mrs_readSTX;
        seqID=-1;
        lastseqID=-1;
        msgID[MSGIDLEN] = 0;
        counter = 0;
    }
  EiQueue msgQueue;
    int init();
    int processMessages();
    void sendMsg(EiMsg &msg);
    EiMsg readMsg();
    void shutdown();
   // int run();

};


const int MSGRECORDSTACKSIZE = 10;




class MsgRecordMgr
{



 /*   typedef struct msgRecordStackEntry
    {
        msgRecord * rec;
        msgRecord * next;
    };
 */
 MsgRecord msgRecords[MSGRECORDSTACKSIZE];
    int low=0;
    int high = 0;
    int lastAdded=0;

   // msgRecordStackEntry stack[MSGRECORDSTACKSIZE]
  public:
    int addMessage(const  char * id, const char * msg, const int len)
    {
        int idx;
        if(lastAdded < MSGRECORDSTACKSIZE)
        {
            lastAdded++;
            high = lastAdded;
       }
        else
        {
            lastAdded = 0;

        }
        idx = lastAdded;

        msgRecords[idx].update(id, len, msg);
        printf("msg added %d\n", idx);
        return 0;

    }
    MsgRecord * nextMessage()
    {
        return 0;
    }
};
#endif
