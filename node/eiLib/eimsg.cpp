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
    int lcopy = fmin(destlen, sourcelen) -1;
    memcpy(dest, source, lcopy);
    dest[lcopy] = 0;
    return lcopy;
}

int msgBody::memcpyn(char *dest, int destlen, char * source, int sourcelen)
{
    int lcopy = fmin(destlen, sourcelen) ;
    memcpy(dest, source, lcopy);
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
  memcpy(&msg[0], name, NAMELEN);
  msg[NAMELEN + 1] = pwdLen;
  memcpy(&msg[NAMELEN+2], pwd, PWDLEN);
  return NAMELEN + PWDLEN + 2;
}
void logon::deserialize( unsigned char * msg)
{
  nameLen = msg[0];
  memcpyn(name, NAMELEN, (char *)&msg[1], nameLen);
  name[nameLen]=0;
  pwdLen = msg[nameLen+1];
  memcpy(pwd, &msg[nameLen+2], pwdLen);
  pwd[pwdLen] = 0;
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
  textLen = strcpyn(this->text, MAXTEXTLEN, text);
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
  text[textLen]=0;
}






int pubsubBase::serialize(unsigned char * msg)
{
    msg[0] = topicLen;
    memcpyn((char *)msg, MAXTOPICLEN, topic, topicLen );
    msg[topicLen + 1] = idLen;
    memcpy((char *)&msg[ topicLen+2], id, idLen);
    msg[topicLen+3+idLen] = psmsgLen;
    memcpy(&msg[topicLen + 3 + idLen],  (char *)psmsg, psmsgLen);
    return topicLen + 3 + idLen + psmsgLen;
}
void pubsubBase::deserialize( unsigned char * msg)
{
    strcpyn(topic, MAXTOPICLEN, psmsg);
    strcpyn(id, MAXIDLEN, &psmsg[ MAXTOPICLEN]);
    memcpyn((char *)msg, MAXPSMSGLEN, &psmsg[MAXTOPICLEN + MAXIDLEN], MAXPSMSGLEN);

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
void get::deserialize( unsigned char * msg)
{
    itemLen = msg[0];
    memcpy(item, &msg[1], itemLen);
    item[itemLen]=0;
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
    get::serialize(msg);
  return 0;
}
void set::deserialize( unsigned char * msg)
{
    get::deserialize(msg);

}


