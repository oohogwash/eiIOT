#include "eiLib/osheader.h"
#include "modulemsg.h"

#include "eiLib/msgdef.h"

using namespace eiMsg;
using namespace eiCom;

namespace eiModule
{

ModuleMsg::ModuleMsg()
{

}

ModuleMsg::ModuleMsg(REST_VERB verb, Module * modules [], int moduleLen) :
    Rest()
{
  this->verb = verb;
  collectionLen = 2;
  SCH ich;
  ich.int16 = htons(moduleLen);
  memcpyn(&collection[collectionLen],2, ich.ch, 2);
  int len;
  for(int idx =0; idx < moduleLen; idx++)
  {
      ich.int16 = htons( modules[idx]->serialize((unsigned char *)&collection[collectionLen+2]));
      memcpyn(&collection[collectionLen], 2, ich.ch,2);
      collectionLen +=  len;
  }
}








} // eiMsgModule
