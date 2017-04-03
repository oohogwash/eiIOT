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

    
}MESSAGE_ID;


class logon : public msgBody
{
  const static int NAMELEN = 10;
  const static int PWDLEN = 10;
public:
  char id(){return mi_Logon;}
  logon();
  logon(char * name, char * pwd);

  char name[10];
  char pwd[10];

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

    char textLen;
    char text[256];
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
 static const int MAXIDLEN = 32;
 char id[MAXIDLEN];
 static const int MAXPSMSGLEN = 256;
 char psmsg[MAXPSMSGLEN];
public:
  pubsubBase(){}
  pubsubBase( char * topic, char * id, char * msg, int msglen )
  {
      strcpyn(this->topic, MAXTOPICLEN, topic);
      strcpyn(this->id, MAXIDLEN, id);
      memcpyn(psmsg, MAXPSMSGLEN, msg, msglen);
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





#endif // EIMSG_H
