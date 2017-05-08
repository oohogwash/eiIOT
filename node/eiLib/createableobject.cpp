#include <stdio.h>
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

void CreateableObject::dump()
{
    printf( "class name = %s ", clsName);
}

int CreateableObject::serialize(unsigned char ** msg)
{
  return serSmallCharArr(msg, clsName, clsidLen);

}
int CreateableObject::deserialize(  unsigned char ** msg)
{
    return deserSmallCharArr(msg, clsName, &clsidLen);

}


}
