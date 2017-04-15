#include "eimsg.h"
#include <string.h>
#include <math.h>
#include "osheader.h"
#include "msgdef.h"

using namespace eiCom;

namespace eiMsg
{

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


int MsgBody::strcpyn(char * dest, int destlen, char * source)
{
    int sourcelen = strlen(source);
    int lcopy = fmin(destlen, sourcelen);
    memcpy(dest, source, lcopy);
    dest[lcopy] = 0;
    return lcopy;
}

int MsgBody::memcpyn(char *dest, int destlen, char * source, int sourcelen, bool addnullterminator)
{
    int lcopy = fmin(destlen, sourcelen) ;
    memcpy(dest, source, lcopy);
    if(addnullterminator)
    {
        dest[lcopy]=0;
    }
    return lcopy;
}



void MsgBody::setToken(unsigned char * token)
{
    tokenLen = strcpyn((char *)this->token, MAXTOKENLEN, (char *)token);
}


unsigned char * MsgBody::serialize(unsigned char * msg)
{
    *msg++ = tokenLen;
    memcpyn((char *)msg, MAXTOKENLEN, (char *)token, tokenLen );
    return msg + tokenLen ;
}

unsigned char * MsgBody::deserialize( unsigned char * msg)
{
    tokenLen =  *msg++;
     memcpyn((char *)token, MAXTOKENLEN, (char *)msg, tokenLen,true);
    return msg + tokenLen;
}





unsigned char * MsgBody::getToken()
{
    return token;
}


Logon::Logon()
{

}

Logon::Logon(char * name, char * pwd)
{
  nameLen = strcpyn(this->name, NAMELEN, name);
  pwdLen = strcpyn(this->pwd, PWDLEN, pwd);
}


unsigned char * Logon::serialize(unsigned char * msg)
{
  msg = MsgBody::serialize(msg);
  msg = serSmallCharArr(msg, name,nameLen);
  msg = serSmallCharArr(msg, pwd, pwdLen); // encrypt this later
  return msg;
}

unsigned char * Logon::deserialize( unsigned char * msg)
{
  msg = MsgBody::deserialize(msg);
  msg = deserSmallString(msg, name, &nameLen);
  msg = deserSmallString(msg, pwd, &pwdLen);
  return msg;
}


unsigned char * EmptyMsgBody::serialize(unsigned char * msg)
{
  return  MsgBody::serialize(msg);
}

unsigned char * EmptyMsgBody::deserialize( unsigned char * msg)
{
    return MsgBody::deserialize(msg);
}

Loopback::Loopback()
{

}

Loopback::Loopback(char * text)
{
  textLen = strcpyn(this->text, MAXTEXTLEN, text);
}


unsigned char * Loopback::serialize(unsigned char * msg)
{
  msg = MsgBody::serialize(msg);
  serSmallString(msg, text);
  return msg;
}
unsigned char * Loopback::deserialize( unsigned char * msg)
{
  msg = MsgBody::deserialize(msg);
  deserSmallString(msg, text, &textLen);
  return msg;
}


unsigned char *  PubsubBase::serialize(unsigned char * msg)
{
    msg = MsgBody::serialize(msg);
    msg = serString(msg,topic);
    msg = serSmallString(msg, id);
    msg = serCharArr(msg, psmsg, psmsgLen);
    return msg;
}

unsigned char *  PubsubBase::deserialize( unsigned char * msg)
{
    msg = MsgBody::deserialize(msg);
    msg = deserString(msg,topic);
    msg = deserSmallString(msg, id);
    msg = deserCharArr(msg, psmsg, &psmsgLen);
    return msg;
}



Get::Get()
{

}

Get::Get( char * item)
{
    itemLen = strcpyn(this->item, MAXITEMLEN, item );
}

unsigned char * Get::serialize(unsigned char * msg)
{
  msg = MsgBody::serialize(msg);
  serCharArr(msg, item, itemLen);
  return msg;
}
unsigned char * Get::deserialize( unsigned char * msg)
{
    msg = MsgBody::deserialize(msg);
    deserCharArr(msg, item, &itemLen);
    return msg;
}


Put::Put()
{
}
Put::Put(char * item, char * data, int len) : Get(item)
{
    infoLen = memcpyn(this->info, MAXDATALEN, data, len);
}


unsigned char * Put::serialize(unsigned char * msg)
{
    msg = Get::serialize(msg);
    msg = serCharArr(msg, info, infoLen);
    return msg;
}

unsigned char * Put::deserialize( unsigned char * msg)
{
    msg = Get::deserialize(msg);
    msg = deserCharArr(msg, info, &infoLen);
    return msg;
}


Rest::Rest()
{

}

Rest::Rest(REST_VERB verb, char * item, char * collection, int collectionLen)
{

}

void Rest::Get(char * item)
{

}

void Rest::Delete(char * item)
{

}

int Rest::Put(char * item, char * collection, int collectionLen)
{
    return 0;
}

int Rest::Post(char * item, char * collection, int collectionLen)
{
    return 0;
}


unsigned char * Rest::serialize(unsigned char * msg)
{
    return msg;
}

unsigned char * Rest::deserialize( unsigned char * msg)
{
    return msg;
}


}// eiMsg
