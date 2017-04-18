#include <string.h>
#include "createableobject.h"
#include "msgdef.h"

using namespace eiCom;

namespace eiKernel
{

CreateableObject::CreateableObject(const  char * clsid)
{
    strcpyn(this->clsName,  MAX_CLSID_LEN, clsid);
}

unsigned char * CreateableObject::serialize(unsigned char * msg)
{
  serSmallCharArr(msg, clsName, clsidLen);
  return msg;
}
unsigned char * CreateableObject::deserialize( unsigned char * msg)
{
    msg =  deserSmallCharArr(msg, clsName, &clsidLen);
    return msg;
}


}
