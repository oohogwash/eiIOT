#include "eimsg.h"
#include <string.h>
#include <math.h>


int msgBody::strcpyn(char * dest, int destlen, char * source)
{
    int sourcelen = strlen(source);
    int lcopy = fmin(destlen, sourcelen) -1;
    memcpy(dest, source, lcopy);
    dest[lcopy] = 0;
    return destlen - sourcelen;
}

int msgBody::memcpyn(char *dest, int destlen, char * source, int sourcelen)
{
    int lcopy = fmin(destlen, sourcelen) ;
    memcpy(dest, source, lcopy);
    return destlen - sourcelen;
}


logon::logon(){}
logon::logon(char * name, char * pwd)
{
  strcpy(this->name, name);
  strcpy(this->pwd, pwd);
}

int logon::serialize(unsigned char * msg)
{
  memcpy(msg, name, NAMELEN);
  memcpy(&msg[NAMELEN], pwd, PWDLEN);
  return NAMELEN + PWDLEN;
}
void logon::deserialize( unsigned char * msg)
{
  memcpy(name, msg, NAMELEN);
  memcpy(pwd, &msg[NAMELEN], PWDLEN);
}



int emptyMsgBody::serialize(unsigned char * msg)
{
  return 0;
}
void emptyMsgBody::deserialize( unsigned char * msg)
{

}

loopback::loopback(){}
loopback::loopback(char * text, char len)
{
  strcpy(this->text, text);
  this->textLen = len;
}

int loopback::serialize(unsigned char * msg)
{
  msg[0] = textLen;
  memcpy(&msg[1], text, textLen);
  return textLen;
}
void loopback::deserialize( unsigned char * msg)
{
  textLen = msg[0];
  memcpy(text, &msg[1], textLen);
}






int pubsubBase::serialize(unsigned char * msg)
{
    strcpyn(psmsg, MAXTOPICLEN, topic );
    strcpyn(&psmsg[ MAXTOPICLEN], MAXIDLEN, id);
    memcpyn(&psmsg[MAXTOPICLEN + MAXIDLEN], MAXPSMSGLEN, (char *)msg,MAXPSMSGLEN);
    return MAXTOPICLEN + MAXIDLEN + MAXPSMSGLEN;
}
void pubsubBase::deserialize( unsigned char * msg)
{
    strcpyn(topic, MAXTOPICLEN, psmsg);
    strcpyn(id, MAXIDLEN, &psmsg[ MAXTOPICLEN]);
    memcpyn((char *)msg, MAXPSMSGLEN, &psmsg[MAXTOPICLEN + MAXIDLEN], MAXPSMSGLEN);

}


