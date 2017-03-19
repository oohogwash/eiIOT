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

#endif // EIMSG_H
