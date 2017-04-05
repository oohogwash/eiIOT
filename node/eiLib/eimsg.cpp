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


logon::logon(){}
logon::logon(char * name, char * pwd)
{
  nameLen = strcpyn(this->name, NAMELEN, name);
  pwdLen = strcpyn(this->pwd, PWDLEN, pwd);
}

int logon::serialize(unsigned char * msg)
{
  msg[0]=nameLen;
  memcpy(&msg[1], name, nameLen);
  msg[nameLen + 1] = pwdLen;
  memcpy(&msg[nameLen+2], pwd, pwdLen);
  return nameLen + pwdLen + 2;
}
int logon::deserialize( unsigned char * msg)
{
  nameLen = msg[0];
  memcpyn(name, NAMELEN, (char *)&msg[1], nameLen, true);
  pwdLen = msg[nameLen+1];
  memcpyn(pwd, PWDLEN, (char *)&msg[nameLen+2], pwdLen, true);
  return nameLen+2+pwdLen;
}



int emptyMsgBody::serialize(unsigned char * msg)
{
  return 0;
}
int emptyMsgBody::deserialize( unsigned char * msg)
{
    return 0;
}

loopback::loopback(){}
loopback::loopback(char * text)
{
  textLen = strcpyn(this->text, MAXTEXTLEN, text);
}

int loopback::serialize(unsigned char * msg)
{
  msg[0] = textLen;
  memcpy(&msg[1], text, textLen);
  return textLen+1;
}
int loopback::deserialize( unsigned char * msg)
{
  textLen = msg[0];
  memcpyn(text, MAXTEXTLEN,(char *) &msg[1], textLen, true);
  return textLen+1;
}






int pubsubBase::serialize(unsigned char * msg)
{
    msg[0] = topicLen;
    memcpyn((char *)&msg[1], MAXTOPICLEN, topic, topicLen );
    msg[topicLen + 1] = idLen;
    memcpy((char *)&msg[ topicLen+2], id, idLen);
    msg[topicLen+2+idLen] = psmsgLen;
    memcpy(&msg[topicLen + 3 + idLen],  (char *)psmsg, psmsgLen);
    return topicLen + 3 + idLen + psmsgLen;
}
int pubsubBase::deserialize( unsigned char * msg)
{
    topicLen = msg[0];
    memcpyn(topic, MAXTOPICLEN, (char *)&msg[1], topicLen,true);
    idLen=msg[topicLen+1];
    memcpyn(id, MAXIDLEN, (char *)&msg[topicLen + 2],idLen, true);
    psmsgLen = msg[topicLen+2+idLen];
    memcpyn((char *)psmsg, MAXPSMSGLEN, (char *)&msg[topicLen + 3 +idLen], psmsgLen);
    return topicLen+3+idLen+ psmsgLen;
}


get::get()
{

}

get::get( char * item)
{
    itemLen = strcpyn(this->item, MAXITEMLEN, item );
}
int get::serialize(unsigned char * msg)
{
  msg[0] = itemLen;
  memcpy(&msg[1], item, itemLen);
  return itemLen + 1;
}
int get::deserialize( unsigned char * msg)
{
    itemLen = msg[0];
    memcpyn(item, MAXITEMLEN, (char *)&msg[1], itemLen, true);
    return itemLen+1;
}
set::set()
{
}
set::set(char * item, char * data, int len) : get(item)
{
    infoLen = memcpyn(this->info, MAXDATALEN, data, len);

}
int set::serialize(unsigned char * msg)
{
    int len = get::serialize(msg);
    return len;
}
int set::deserialize( unsigned char * msg)
{
    int len = get::deserialize(msg);

    return len;
}


