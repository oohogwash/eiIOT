#ifndef EICOM_H_
#define EICOM_H_

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

   // STX, HDRLEN, MSGID, MSGLEN,SECID, MSGBODY,  ETX

// msg and header item sizes
static const int MAXMSGLEN = 512;
static const int MSGIDLEN = 4;
static const int MSGLENLEN = 4;
static const int MSGSECIDLEN = 4;
static const int MSGCODELEN = 1;
static const int MSGHDRLENLEN = 1;
static const int MSGSEQIDLEN = 1;

enum hdroffsets
{
    MSGSTXOFFSET    = 0,
		MSGCODEOFFSET   = MSGSTXOFFSET + 1,
    MSGHDRLENOFFSET = MSGCODEOFFSET + MSGCODELEN,
    MSGIDOFFSET     = MSGHDRLENOFFSET + MSGHDRLENLEN,
		MSGSEQIDOFFSET  = MSGIDOFFSET + MSGIDLEN,
    MSGLENOFFSET    = MSGSEQIDOFFSET       + MSGSEQIDLEN,
    MSGSECIDOFFSET  = MSGLENOFFSET      + MSGLENLEN,
    MSGBODYOFFSET   = MSGSECIDOFFSET    + MSGSECIDLEN
};


//msg values
static char STX = 0x02;
static char ETX = 0x03;
static char eiMsgID = 'H';
static const int MAXBODYLEN = MAXMSGLEN - MSGBODYOFFSET;
static const char MSGHDRLEN = MSGBODYOFFSET;

typedef struct msgHeader
{
    int hdrlen;
    char eiMsgCode;
    char msgID[MSGIDLEN+1];
    long msgLen;
    char secid[MSGSECIDLEN + 1];
}MSGHEADER;
typedef union _val {
    char buff[8];
    long lval;
}VAL;

class eiMsg
{
    char _msgBuffer[MAXMSGLEN +1];
    long _len;
    char _id[MSGIDLEN+1];
		static char sequenceIDin;//iterate from 0..255 for each msg sent
		static char sequenceIDout; //iterate from 0..255 for each msg recd
		char getSequenceID();
		bool checkSequenceID(char sequenceID);
public:
    eiMsg();

    char * body();
    char *  msgID();
    long len(void);

    const char * msg(){return _msgBuffer;}

    long setBody(const char * msgId, const void * body, long len);
    void setLen(const long msgLen);
    void setID(const char * id);
    void dump();

};


class eiCom
{
public:
    eiCom(){}
    void init(){}
    void run(){}
    eiMsg readMsg();
    void sendMsg(eiMsg & msg);
};





#endif
