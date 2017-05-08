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
    {mi_PingResponse, "Ping response", "Response to a previous ping", msna_ignore},
    {mi_Loopback, "Loopback", "Loopback test", msna_ignore},
    {mi_LoopbackResponse, "Loopback response", "Response to loopback message", msna_ignore},
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

void MsgBody::setToken(const unsigned char * token)
{
    strcpy((char *)this->token, (char *)token);
}

int MsgBody::serialize(unsigned char ** msg)
{
    int sz = serSmallString(msg, (char *)token);
    sz += serUChar(msg, numNVP);
    for(int idx = 0; idx < numNVP; idx++)
    {
        sz += serSmallString(msg, nvp[idx].name);
        sz += serSmallString(msg, nvp[idx].value);
     }
    return sz;
}

int MsgBody::deserialize(  unsigned char ** msg)
{
    int sz = deserSmallString(msg, (char *)token);
    sz += deserUChar(msg, &numNVP);
    for(int idx = 0; idx < numNVP; idx++)
    {
        sz += deserSmallString(msg, nvp[idx].name);
        sz += deserSmallString(msg, nvp[idx].value);
     }
    return sz;
}

int MsgBody::setNameValuePair(char *name, char *value)
{
    for(int idx = 0; idx < numNVP; idx++)
    {
        if(!strcmp(nvp[idx].name, name) )
        {
            strcpy(nvp[idx].value,value);
            return idx;
        }
    }
    if(numNVP < NUMNVP)
    {
        strcpy(nvp[numNVP].name, name);
        strcpy(nvp[numNVP++].value, value);
        return numNVP;
    }
    else
    {
        return -1; // overflow
    }
}
char * MsgBody::getNameValuePair(char *name)
{
    for(int idx = 0; idx < numNVP; idx++)
    {
        if(!strcmp(nvp[idx].name, name) )
        {
            return nvp[idx].value;
        }


    }
    return NULL; // not found
}

const unsigned char * MsgBody::getToken() const
{
    return token;
}

Logon::Logon()
{

}

Logon::Logon(char * name, char * pwd, char * domain)
{
  strcpy(this->name, name);
  strcpy(this->pwd,  pwd);
  strcpy(this->domain, domain);
}


int Logon::serialize(unsigned char ** msg)
{
  int sz = MsgBody::serialize(msg);
  sz += serSmallString(msg, name);
  sz += serSmallString(msg, pwd); // encrypt this later
  sz += serSmallString(msg, domain);
  return sz;
}

int Logon::deserialize(  unsigned char ** msg)
{
  int sz = MsgBody::deserialize(msg);
  sz += deserSmallString(msg, name);
  sz += deserSmallString(msg, pwd);
  sz += deserSmallString(msg, domain);
  return sz;
}


int EmptyMsgBody::serialize(unsigned char ** msg)
{
  return  MsgBody::serialize(msg);
}

int EmptyMsgBody::deserialize( unsigned char ** msg)
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


int Loopback::serialize(unsigned char ** msg)
{
  int sz = MsgBody::serialize(msg);
  sz += serSmallString(msg, text);
  return sz;
}
int Loopback::deserialize(  unsigned char ** msg)
{
  int sz = MsgBody::deserialize(msg);
  sz += deserSmallString(msg, text, &textLen);
  return sz;
}
LoopbackResponse::LoopbackResponse()
{

}

LoopbackResponse::LoopbackResponse(char * text)
{
  textLen = strcpyn(this->text, MAXTEXTLEN, text);
}

int LoopbackResponse::serialize(unsigned char ** msg)
{
  int sz = MsgBody::serialize(msg);
  sz += serSmallString(msg, text);
  return sz;
}
int LoopbackResponse::deserialize(  unsigned char ** msg)
{
  int sz = MsgBody::deserialize(msg);
  sz += deserSmallString(msg, text, &textLen);
  return sz;
}

int PubsubBase::serialize(unsigned char ** msg)
{
    int sz = MsgBody::serialize(msg);
    sz += serString(msg,topic);
    sz += serSmallString(msg, id);
    sz += serCharArr(msg, psmsg, psmsgLen);
    return sz;
}

int  PubsubBase::deserialize(  unsigned char ** msg)
{
    int sz = MsgBody::deserialize(msg);
    sz += deserString(msg,topic);
    sz += deserSmallString(msg, id);
    sz += deserCharArr(msg, psmsg, &psmsgLen);
    return sz;
}



Get::Get()
{

}

Get::Get( char * item)
{
   strcpy(this->item, item );
}

int Get::serialize(unsigned char ** msg)
{
  int sz = MsgBody::serialize(msg);
  sz += serSmallString(msg, item);
  return sz;
}
int Get::deserialize(  unsigned char ** msg)
{
    int sz = MsgBody::deserialize(msg);
    sz += deserSmallString(msg, item);
    return sz;
}
void Get::setItem(char *item)
{
    strcpy(this->item, item);
}



Put::Put()
{
}
Put::Put(char * item, char * data, int len) : Get(item)
{
    infoLen = memcpyn(this->info, MAXDATALEN, data, len);
}


int Put::serialize(unsigned char ** msg)
{
    int sz = Get::serialize(msg);
    sz += serCharArr(msg, info, infoLen);
    return sz;
}

int Put::deserialize(  unsigned char ** msg)
{
    int sz = Get::deserialize(msg);
    sz += deserCharArr(msg, info, &infoLen);
    return sz;
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


int Rest::serialize(unsigned char ** msg)
{
    return 0;
}

int Rest::deserialize(  unsigned char ** msg)
{
    return 0;
}


}// eiMsg
