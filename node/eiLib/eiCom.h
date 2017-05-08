#ifndef EICOM_H_
#define EICOM_H_


#include "eimsg.h"
#include "eiqueue.h"

#include "comIO.h"
namespace eiMsg
{

char * itoa(int i, char buff[]);

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
    unsigned char token[MsgBody::MAXTOKENLEN + 1];
    int msgBodyLen;

    msgReadState msgState;
    unsigned char seqID;
    unsigned char lastseqID;
    int msgIdx;
    char msgID[MSGIDLEN+1];
    unsigned char MSG[MAXMSGLEN+1];
    static const int READBUFFER_SIZE  = 2048;
    unsigned char readBuffer[READBUFFER_SIZE+1];
    static const int MAXSEQNUM = 10;
    static const int MINSEQNUM = 1;
    int16_t counter ;
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
        token[0] = NULL;
    }
    EiQueue msgQueue;
    void setToken(unsigned char * newToken);
    int init();
    int processMessages();
    void sendMsg(EiMsg &msg);
    void sendMsg(const int16_t msgId, const void * body, const int16_t len);
    void sendMsg(const unsigned char * msgId, const void * body, const int16_t len);
    void sendMsg(const  unsigned char * msgId,  MsgBody * msgbody);
    void sendMsg(const int16_t msgId,  MsgBody * msgBody);
    void sendMsg(const   char * msgId,  MsgBody * msgbody)
        {return sendMsg((const  unsigned char *) msgId,  msgbody);}
    void sendMsg( MsgBody & msgbody);

    void shutdown();
};


} // eiMsg

#endif
