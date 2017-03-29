#include "eimsg.h"


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
