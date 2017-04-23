#include "osheader.h"
#include "modulemsg.h"
#include "objectfactory.h"
#include "msgdef.h"
#include "createableobject.h"
#include "logicmodule.h"

using namespace eiMsg;
using namespace eiCom;
using namespace eiKernel;

namespace eiModule
{

ModuleMsg::ModuleMsg() : Rest()
{

}

unsigned char * ModuleMsg::serialize(REST_VERB verb, Module * modules , int moduleLen)
{
  this->verb = verb;
  unsigned char * msg = collection;
  msg = serUChar(msg, verb);
  msg = serInt16(msg, moduleLen);
  for(int idx =0; idx < moduleLen; idx++)
  {
    msg = serSmallString(msg, modules[idx].clsName); //there are derived types of modules so serialise clsid values
  }
  for(int idx =0; idx < moduleLen; idx++)
  {
      msg =  modules[idx].serialize(msg);
  }




  collectionLen = msg-collection;
  return msg;
}

unsigned char * ModuleMsg::deserialize (unsigned char * msg)
{
    //Note this method creates new array and new Module pointers in the array
    msg = deserUChar(msg, (unsigned char *)&verb);
    msg = deserInt16(msg,  (int16_t *)&modulesLen);
    modules = new Module * [modulesLen];
    //int x = modulesLen;
    //char moduleClsid[x][CreateableObject::MAX_CLSID_LEN];
    char ** moduleClsid = new char * [modulesLen];

    for(int idx =0; idx < modulesLen; idx++)
    {
      moduleClsid[idx] = new char[CreateableObject::MAX_CLSID_LEN];
      msg = deserSmallString( msg , moduleClsid[idx]); //there are derived types of modules so serialise clsid values
    }
    for(int idx =0; idx < modulesLen; idx++)
    {
        modules[idx] =  ( Module *) eiKernel::ObjectFactory::getObject((char *)moduleClsid[idx]);
        delete(moduleClsid[idx]);
        msg = modules[idx]->deserialize(msg);
        modules[idx]->dump();
    }
    collectionLen = msg - collection;
    return msg;
}






} // eiMsgModule
