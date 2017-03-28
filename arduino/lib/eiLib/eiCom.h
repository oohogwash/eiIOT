#ifndef EICOM_H_
#define EICOM_H_

#include "comIO.h"
#include "eimsg.h"
#include "msgrecord.h"
#include "eiqueue.h"
#include <stdio.h>

typedef enum _msgID
{
	Subscribe,
    Notify,
    Cmd
}MSGID;

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
class eiCom
{

	public:
			static const int READBUFFER_SIZE  = 2048;
		private:
    enum msgReadState
    {
      mrs_readSTX,
      mrs_readType,
        mrs_ReadHdrLen,
        mrs_ReadHdr,
        mrs_ReadBody

    }mrs;
    comIO  *  io;

    int msgBodyLen;

    msgReadState msgState;
    int seqID;
    int lastseqID;
    char msgID[MSGIDLEN];
    char MSG[MAXMSGLEN];
    unsigned char readBuffer[READBUFFER_SIZE];
    static const int MAXSEQNUM = 10;
    static const int MINSEQNUM = 1;
public:

    eiCom( comIO * io = 0)
    {
        this->io = io;
        msgState = mrs_readSTX;
        seqID=-1;
        lastseqID=-1;
        msgID[MSGIDLEN] = 0;
    }
  eiQueue msgQueue;
    int init();
    int processMessages();
    void sendMsg(eiMsg &msg);
    eiMsg readMsg();
    void shutdown();
		void setIO(comIO * io){this->io = io;};
;

};




#endif
