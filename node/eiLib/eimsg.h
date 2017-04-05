#ifndef EIMSG_H
#define EIMSG_H



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


enum MsgSeqNumAction
{
    msna_ignore,        // do nothing
    msna_set,           // set the current sequence number to the number in the msg
    msna_check_warn,    // check the seq number if it does not match warn but still process
    msna_check_error    // check the seq number if it does not match reject msg
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
    char eiMsgCode[MSGCODELEN+1];
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
public:
    eiMsg();

    char * body();
    char *  msgID();
    long len(void);
    static char sequenceIDin;//iterate from 0..255 for each msg sent
    static char sequenceIDout; //iterate from 0..255 for each msg recd
    char getSequenceID();
    const char * msg(){return _msgBuffer;}

    long setBody(const char * msgId, const void * body, long len);
    void setLen(const long msgLen);
    void setID(const char * id);
    void dump();

};



class msgBody
{
   public:
  static int memcpyn(char *dest, int destlen, char * source, int sourcelen);
  static int strcpyn(char * dest, int destlen, char * source);
  virtual char id() = 0;
  static const int TOKENLEN = 4;
  unsigned char token[TOKENLEN];
  virtual int serialize(unsigned char * msg) =0;
  virtual void deserialize( unsigned char * msg)=0;

};




class emptyMsgBody : public msgBody
{

public:
  emptyMsgBody(){}
  int serialize(unsigned char * msg);
  void deserialize( unsigned char * msg);


};

////////////////////////////////////////////////////////////////////////////////////////////////
/// The message definitions
///
/// 

typedef enum MessageId
{
    mi_User=200,
    mi_Sys=50,
    mi_Logon,
    mi_LogonResponse,
    mi_Logoff,
    mi_Ping,
    mi_Loopback,
    mi_Publish,
    mi_Subscribe,
    mi_Notify,
    mi_Publications,
    mi_Command,
    mi_CommandResponse,
    mi_Request,
    mi_Response,
    mi_Get,
    mi_Set

    
}MESSAGE_ID;

typedef struct msginfo{
    MESSAGE_ID id;
    const char * name;
    const char * desc;
    MsgSeqNumAction action;
}MSGINFO;









class logon : public msgBody
{
  const static int NAMELEN = 10;
  const static int PWDLEN = 10;
public:
  char id(){return mi_Logon;}
  logon();
  logon(char * name, char * pwd);

  char name[10];
  char nameLen;
  char pwd[10];
  char pwdLen;

  int serialize(unsigned char * msg);
  void deserialize( unsigned char * msg);
};


class logonResponse : public emptyMsgBody
{
public:
     char id(){return mi_LogonResponse;}
    logonResponse(){}
};


class logOff : public emptyMsgBody
{
public:
     char id(){return mi_Logoff;}
    logOff(){}
};



class ping : public emptyMsgBody
{
public:
  char id(){return mi_Ping;}
  ping(){}
};


class loopback : public msgBody
{
    static const int MAXTEXTLEN = 256;
    char textLen;
    char text[MAXTEXTLEN];

public:
  char id(){return mi_Loopback;}
  loopback();
  loopback(char * text, char lenText);
  int serialize(unsigned char * msg);
  void deserialize( unsigned char * msg);
};

class pubsubBase : public msgBody
{
 static const int MAXTOPICLEN = 128;
 char topic[MAXTOPICLEN];
 char topicLen;
 static const int MAXIDLEN = 32;
 char id[MAXIDLEN];
 char idLen;
 static const int MAXPSMSGLEN = 256;
 char psmsg[MAXPSMSGLEN];
 int psmsgLen;

public:
  pubsubBase(){}
  pubsubBase( char * topic, char * id, char * msg, int msglen )
  {
      topicLen = strcpyn(this->topic, MAXTOPICLEN, topic);
      idLen = strcpyn(this->id, MAXIDLEN, id);
      psmsgLen =  memcpyn(psmsg, MAXPSMSGLEN, msg, msglen);
  }
  int serialize(unsigned char * msg);
  void deserialize( unsigned char * msg);
};



class publish : public pubsubBase
{

public:
  char id(){return mi_Publish;}
  publish(){}
  publish(char * topic, char * id, char * msg, int len):
      pubsubBase(  topic, id, msg, len){}
};

class subscribe : public pubsubBase
{

public:
  char id(){return mi_Subscribe;}
  subscribe(){}
  subscribe(char * topic, char * id, char * msg, int len) :
      pubsubBase(  topic, id, msg, len){}
};
class notify : public pubsubBase
{

public:
  char id(){return mi_Notify;}
  notify(){}
  notify(char * topic, char * eventId, char * event, int eventLen):
    pubsubBase(  topic, eventId, event, eventLen){}
};

class command : public msgBody
{

public:
  char id(){return mi_Command;}
  command( char * id, char * cmd, int len);
};

class commandResponse : public msgBody
{

public:
    char id(){return mi_CommandResponse;}
    commandResponse();
};


class request : public msgBody
{

public:
  char id(){return mi_Request;}
  request( char * id, char * cmd, int len);
};

class response : public msgBody
{
public:
    char id(){return mi_Response;}
    response();
};

class get : public msgBody
{
    static const int MAXITEMLEN = 128;
    char item[MAXITEMLEN];
    char itemLen;

public:
  char id(){return mi_Get;}
  get();
  get( char * item);
  int serialize(unsigned char * msg);
  void deserialize( unsigned char * msg);
};

class set : public get
{
    static const int MAXDATALEN = 1024;
    char info[MAXDATALEN];
    int infoLen;
public:
    char id(){return mi_Set;}
    set();
    set(char * item, char * info, int len);
    int serialize(unsigned char * msg);
    void deserialize( unsigned char * msg);
};





#endif // EIMSG_H
