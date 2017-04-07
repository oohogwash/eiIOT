#include "eimsg.h"
#include <string.h>
#include <math.h>


#include <windows.h>


MSGINFO messageInfo [] = {
    {mi_Logon, "Logon", "Log onto server.", msna_set},
    {mi_LogonResponse, "Logon response.", "Response from logon request.", msna_set},
    {mi_Logoff, "Logoff", "Log off session.", msna_ignore},
    {mi_Ping, "Ping", "Ping test", msna_ignore},
    {mi_Loopback, "Loopback", "Loopback test", msna_ignore},
    {mi_Publish, "Publish", "Publish information to topic", msna_check_warn},
    {mi_Subscribe, "Subscribe", "Subscribe to messages published on topic.", msna_check_warn},
    {mi_Notify, "Notify", "Notify event occurance.", msna_check_warn},
    {mi_Publications, "Publications", "List all publications.", msna_check_warn},
    {mi_Command, "Command", "Execute command.", msna_check_warn},
    {mi_CommandResponse, "Command response", "Response from executed command", msna_check_warn},
    {mi_Request, "Request", "Request action", msna_check_warn},
    {mi_Response, "Response", "Request response.", msna_check_warn},
    {mi_Get, "Get", "Get information", msna_check_warn},
    {mi_Set, "Set", "Set information", msna_check_warn}

};


int msgBody::strcpyn(char * dest, int destlen, char * source)
{
    int sourcelen = strlen(source);
    int lcopy = fmin(destlen, sourcelen);
    memcpy(dest, source, lcopy);
    dest[lcopy] = 0;
    return lcopy;
}

int msgBody::memcpyn(char *dest, int destlen, char * source, int sourcelen, bool addnullterminator)
{
    int lcopy = fmin(destlen, sourcelen) ;
    memcpy(dest, source, lcopy);
    if(addnullterminator)
    {
        dest[lcopy]=0;
    }
    return lcopy;
}

#include <stdio.h>
int msgBody::serialize(unsigned char * msg)
{
    msg[0] = tokenLen;
    memcpyn((char *)&msg[1], MAXTOKENLEN, (char *)token, tokenLen );
    return tokenLen + 1;
}
int msgBody::deserialize( unsigned char * msg)
{
    tokenLen =  msg[0];
    memcpyn((char *)token, MAXTOKENLEN, (char *)&msg[1], tokenLen,true);
    return tokenLen+1;
}




logon::logon(){}
logon::logon(char * name, char * pwd)
{
  nameLen = strcpyn(this->name, NAMELEN, name);
  pwdLen = strcpyn(this->pwd, PWDLEN, pwd);
}

int logon::serialize(unsigned char * msg)
{
  int bodyLen = msgBody::serialize(msg);
  msg[bodyLen]=nameLen;
  memcpy(&msg[1+ bodyLen], name, nameLen);
  msg[nameLen + 1 + bodyLen] = pwdLen;
  memcpy(&msg[nameLen+2 + bodyLen], pwd, pwdLen);
  return nameLen + pwdLen + 2 + bodyLen;
}
int logon::deserialize( unsigned char * msg)
{
    int bodyLen = msgBody::deserialize(msg);
  nameLen = msg[bodyLen];
  memcpyn(name, NAMELEN, (char *)&msg[1 + bodyLen], nameLen, true);
  pwdLen = msg[nameLen+1+ bodyLen];
  memcpyn(pwd, PWDLEN, (char *)&msg[nameLen+2+bodyLen], pwdLen, true);
  return nameLen+2+pwdLen + bodyLen;
}



int emptyMsgBody::serialize(unsigned char * msg)
{
  return  msgBody::serialize(msg);
}
int emptyMsgBody::deserialize( unsigned char * msg)
{
    return msgBody::deserialize(msg);
}

loopback::loopback(){}
loopback::loopback(char * text)
{
  textLen = strcpyn(this->text, MAXTEXTLEN, text);
}

int loopback::serialize(unsigned char * msg)
{
    int bodyLen = msgBody::serialize(msg);
  msg[bodyLen] = textLen;
  memcpy(&msg[1+ bodyLen], text, textLen);
  return textLen+1+ bodyLen;
}
int loopback::deserialize( unsigned char * msg)
{
    int bodyLen = msgBody::deserialize(msg);
  textLen = msg[bodyLen];
  memcpyn(text, MAXTEXTLEN,(char *) &msg[1+ bodyLen], textLen, true);
  return textLen+1+ bodyLen;
}






int pubsubBase::serialize(unsigned char * msg)
{
      int bodyLen = msgBody::serialize(msg);
    msg[bodyLen] = topicLen;
    memcpyn((char *)&msg[1 + bodyLen], MAXTOPICLEN, topic, topicLen );
    msg[topicLen + 1 + bodyLen] = idLen;
    memcpy((char *)&msg[ topicLen+2 + bodyLen], id, idLen);
    msg[topicLen+2+idLen + bodyLen] = psmsgLen;
    memcpy(&msg[topicLen + 3 + idLen + bodyLen],  (char *)psmsg, psmsgLen);
    return topicLen + 3 + idLen + psmsgLen + bodyLen;
}
int pubsubBase::deserialize( unsigned char * msg)
{
    int bodyLen = msgBody::deserialize(msg);
    topicLen = msg[bodyLen];
    memcpyn(topic, MAXTOPICLEN, (char *)&msg[1+bodyLen], topicLen,true);
    idLen=msg[topicLen+1+bodyLen];
    memcpyn(id, MAXIDLEN, (char *)&msg[topicLen + 2+bodyLen],idLen, true);
    psmsgLen = msg[topicLen+2+idLen+bodyLen];
    memcpyn((char *)psmsg, MAXPSMSGLEN, (char *)&msg[topicLen + 3 +idLen+bodyLen], psmsgLen);
    return topicLen+3+idLen+ psmsgLen+bodyLen;
}

#include <stdio.h>

get::get()
{

}

get::get( char * item)
{
    itemLen = strcpyn(this->item, MAXITEMLEN, item );
}
int get::serialize(unsigned char * msg)
{
  int bodyLen = msgBody::serialize(msg);
  printf("len = %d ", bodyLen);

  msg[bodyLen] = itemLen;
  memcpy(&msg[1 + bodyLen], item, itemLen);
  return itemLen + 1 + bodyLen;
}
int get::deserialize( unsigned char * msg)
{
    int bodyLen = msgBody::deserialize(msg);
    printf("len =-=> %d ", bodyLen);
    itemLen = msg[bodyLen];
    memcpyn(item, MAXITEMLEN, (char *)&msg[1+ bodyLen], itemLen, true);
    return itemLen+1 + bodyLen;
}
set::set()
{
}
set::set(char * item, char * data, int len) : get(item)
{
    infoLen = memcpyn(this->info, MAXDATALEN, data, len);
printf("infolen=%d", infoLen);
}

int set::serialize(unsigned char * msg)
{
    int len = get::serialize(msg);

    msg[len+1] = infoLen;
    memcpyn((char *)&msg[len+2], MAXDATALEN, info, infoLen);
    return len +1 + infoLen;
}
int set::deserialize( unsigned char * msg)
{

    int len = get::deserialize(msg);
    printf("len = %d ", len);
    infoLen = msg[len+1];
    printf("len = %d ", infoLen);

    memcpyn(info, MAXDATALEN, (char *)&msg[len+2], infoLen);
    printf("[[[%4s]]]", info);
    return len +1 + infoLen;
}


