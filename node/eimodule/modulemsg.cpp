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

int ModuleMsg::serialize(unsigned char **msg)
{
    return 0;
}

int ModuleMsg::serialize(  unsigned char ** msg, REST_VERB verb, Module * modules , int moduleLen)
{
  this->verb = verb;
  unsigned char * start = *msg;
  if( msg == 0)
  {
    *msg = collection;
    start = collection;
  }
  int sz = serUChar(msg, verb);
  sz += serInt16(msg, moduleLen);
  for(int idx =0; idx < moduleLen; idx++)
  {
    sz += serSmallString(msg, modules[idx].clsName); //there are derived types of modules so serialise clsid values
  }
  for(int idx =0; idx < moduleLen; idx++)
  {
      sz +=  modules[idx].serialize(msg);
  }




  collectionLen = sz;

  return sz;
}

int ModuleMsg::deserialize ( unsigned char ** msg)
{
    //Note this method creates new array and new Module pointers in the array
    int sz = deserUChar(msg, (unsigned char *)&verb);
    sz += deserInt16(msg,  (int16_t *)&modulesLen);
    modules = new Module * [modulesLen];
    //int x = modulesLen;
    //char moduleClsid[x][CreateableObject::MAX_CLSID_LEN];
    char ** moduleClsid = new char * [modulesLen];

    for(int idx =0; idx < modulesLen; idx++)
    {
      moduleClsid[idx] = new char[CreateableObject::MAX_CLSID_LEN];
      sz += deserSmallString( msg , moduleClsid[idx]); //there are derived types of modules so serialise clsid values
    }
    for(int idx =0; idx < modulesLen; idx++)
    {
        modules[idx] =  ( Module *) eiKernel::ObjectFactory::getObject((char *)moduleClsid[idx]);
        delete(moduleClsid[idx]);
        sz += modules[idx]->deserialize(msg);
       // modules[idx]->dump();
    }
    collectionLen = sz;
    return sz;
}






} // eiMsgModule
