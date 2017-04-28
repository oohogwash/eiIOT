#ifndef EIMSG_H
#define EIMSG_H
#include <inttypes.h>
#include "createableobject.h"

namespace eiMsg
{

typedef enum tag_REST_VERB : unsigned char
{
    rv_GET,
    rv_PUT,
    rv_POST,
    rv_DELETE
}REST_VERB;


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




class MsgBody : public eiKernel::CreateableObject
{
   public:
  static int memcpyn(char *dest, int destlen, char * source, int sourcelen, bool addNullTerminator=false);
  static int strcpyn(char * dest, int destlen, char * source);
  virtual char mid() = 0;
  static const int MAXTOKENLEN = 4;
  unsigned char token[MAXTOKENLEN+1];
  unsigned char tokenLen;
  virtual unsigned char * serialize(unsigned char * msg);
  virtual unsigned char * deserialize( unsigned char * msg);
  MsgBody(): tokenLen(0){}
  void setToken(unsigned char * token);
  unsigned char * getToken();

};

class EiMsg
{
    unsigned char eiMsgID;
    unsigned char _msgBuffer[MAXMSGLEN +1];
    int16_t _msglen;
    unsigned char _id[MSGIDLEN+1];
public:
    EiMsg();

    unsigned char * body();
    unsigned char *  msgID();
    int16_t len(void);
    static unsigned char sequenceIDin;//iterate from 0..255 for each msg sent
    static unsigned char sequenceIDout; //iterate from 0..255 for each msg recd
    unsigned char getSequenceID();
    const unsigned char * msg(){return _msgBuffer;}
    long setBody(const  char * msgId, const void * body, long len)
    { return setBody( (unsigned char * ) msgId, body, len);}
    long setBody(const  unsigned char * msgId, MsgBody * msgbody);
    long setBody(const   char * msgId, MsgBody * msgbody)
    {return setBody((const  unsigned char *) msgId,  msgbody);}

    long setBody(const unsigned char * msgId, const void * body, long len);
    void setLen(const long msgLen);
    void setID( const unsigned char * id);
    void dump();

};




class EmptyMsgBody : public MsgBody
{

public:
  EmptyMsgBody(){}
  unsigned char *  serialize(unsigned char * msg);
  unsigned char *  deserialize( unsigned char * msg);


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
    mi_Set,
    mi_Rest

    
}MESSAGE_ID;

typedef struct msginfo{
    MESSAGE_ID id;
    const char * name;
    const char * desc;
    MsgSeqNumAction action;
}MSGINFO;




class Logon : public MsgBody
{
  const static int NAMELEN = 40;
  const static int PWDLEN = 40;
public:
  char mid(){return mi_Logon;}
  Logon();
  Logon(char * name, char * pwd);

  char name[NAMELEN+1];
  unsigned char nameLen;
  char pwd[PWDLEN+1];
  unsigned char pwdLen;
  unsigned char * serialize(unsigned char * msg);
  unsigned char * deserialize( unsigned char * msg);
};


class LogonResponse : public EmptyMsgBody
{
public:
     char mid(){return mi_LogonResponse;}
    LogonResponse(){}
};


class LogOff : public EmptyMsgBody
{
public:
     char mid(){return mi_Logoff;}
    LogOff(){}
};



class Ping : public EmptyMsgBody
{
public:
  char mid(){return mi_Ping;}
  Ping(){}
};


class Loopback : public MsgBody
{
    static const int MAXTEXTLEN = 256;
    unsigned char textLen;

public:
  char text[MAXTEXTLEN+1];
  char mid(){return mi_Loopback;}
  Loopback();
  Loopback(char * text);
  unsigned char * serialize(unsigned char * msg);
  unsigned char * deserialize( unsigned char * msg);

};


class PubsubBase : public MsgBody
{

public:

 static const int MAXTOPICLEN = 128;
 char topic[MAXTOPICLEN+1];
 char topicLen;
 static const int MAXIDLEN = 32;
 char id[MAXIDLEN+1];
 char idLen;
 static const int MAXPSMSGLEN = 256;
 char psmsg[MAXPSMSGLEN+1];
 uint16_t psmsgLen;

  PubsubBase(){}
  PubsubBase( char * topic, char * id, char * msg, int msglen )
  {
      topicLen = strcpyn(this->topic, MAXTOPICLEN, topic);
      idLen = strcpyn(this->id, MAXIDLEN, id);
      psmsgLen =  memcpyn(psmsg, MAXPSMSGLEN, msg, msglen);
  }
  unsigned char * serialize(unsigned char * msg);
  unsigned char * deserialize( unsigned char * msg);

};



class Publish : public PubsubBase
{

public:
  char mid(){return mi_Publish;}
  Publish(){}
  Publish(char * topic, char * id, char * msg, int len):
      PubsubBase(  topic, id, msg, len){}
};

class Subscribe : public PubsubBase
{

public:
  char mid(){return mi_Subscribe;}
  Subscribe(){}
  Subscribe(char * topic, char * id, char * msg, int len) :
      PubsubBase(  topic, id, msg, len){}
};

class Notify : public PubsubBase
{

public:
  char mid(){return mi_Notify;}
  Notify(){}
  Notify(char * topic, char * eventId, char * event, int eventLen):
    PubsubBase(  topic, eventId, event, eventLen){}
};

class Command : public MsgBody
{

public:
  char mid(){return mi_Command;}
  Command( char * id, char * cmd, int len);
};

class CommandResponse : public MsgBody
{

public:
    char mid(){return mi_CommandResponse;}
    CommandResponse();
};


class Request : public MsgBody
{

public:
  char mid(){return mi_Request;}
  Request( char * id, char * cmd, int len);
};

class Response : public MsgBody
{
public:
    char mid(){return mi_Response;}
    Response();
};

class Get : public MsgBody
{
    static const int MAXITEMLEN = 128;


public:
    char item[MAXITEMLEN+1];
    uint16_t itemLen;
    char mid(){return mi_Get;}
  Get();
  Get( char * item);
  unsigned char * serialize(unsigned char * msg);
  unsigned char * deserialize( unsigned char * msg);

};

class Put : public Get
{
    static const int MAXDATALEN = 1024;

public:
    char info[MAXDATALEN+1];
    uint16_t infoLen;
    char mid(){return mi_Set;}
    Put();
    Put(char * item, char * info, int len);
    unsigned char * serialize(unsigned char * msg);
    unsigned char * deserialize( unsigned char * msg);

};

class Post : public Put
{
    static const int MAXDATALEN = 1024;

public:
    char info[MAXDATALEN+1];
    int infoLen;
    char mid(){return mi_Set;}
    Post();
    Post(char * item, char * info, int len);
    unsigned char * serialize(unsigned char * msg);
    unsigned char * deserialize( unsigned char * msg);

};

class Delete : public Get
{

};

class Rest : public MsgBody
{
    static const int MAXCOLLECTIOMDATALEN = 1024;
public:
    unsigned char collection[MAXCOLLECTIOMDATALEN+1];
    int collectionLen;
    REST_VERB verb;
    char mid(){return mi_Rest;}
    Rest();
    Rest(REST_VERB verb, char * item, char * collection, int collectionLen);
    void Get(char * item);
    void Delete(char * item);
    int Put(char * item, char * collection, int collectionLen);
    int Post(char * item, char * collection, int collectionLen);
    unsigned char * serialize(unsigned char * msg);
    unsigned char * deserialize( unsigned char * msg);


};

}; //eiMsg


#endif // EIMsg_H
