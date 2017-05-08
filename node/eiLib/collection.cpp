#include <inttypes.h>
#include "collection.h"
#include "objectfactory.h"
#include "msgdef.h"
#include "createableobject.h"





using namespace eiMsg;
using namespace eiCom;
using namespace eiKernel;

namespace eiMsg
{


Collection::Collection()
{

}

int Collection::serialize(unsigned char **msg)
{
    return 0;
}

int Collection::serialize(  unsigned char ** msg, CreateableObject * objects , int objectLen)
{
  unsigned char * start = *msg;
  if(msg == 0)
  {
    *msg = collection;
    start = collection;
  }

  int sz = serInt16(msg, objectLen);
  for(int idx =0; idx < objectLen; idx++)
  {
    sz += serSmallString(msg, objects[idx].clsName); //there are derived types of objects so serialise clsid values
  }
  for(int idx =0; idx < objectLen; idx++)
  {
      sz += objects[idx].serialize(msg);
  }
  collectionLen = sz;
  return sz;
}

int Collection::deserialize ( unsigned char ** msg)
{
    //Note this method creates new array and new object pointers in the array
    int sz = deserInt16(msg,  (int16_t *)&objectsLen);
    objects = new CreateableObject * [objectsLen];
    char ** objectClsid = new char * [objectsLen];

    for(int idx =0; idx < objectsLen; idx++)
    {
      objectClsid[idx] = new char[CreateableObject::MAX_CLSID_LEN];
      sz += deserSmallString( msg , objectClsid[idx]); //there are derived types of objects so serialise clsid values
    }
    for(int idx =0; idx < objectsLen; idx++)
    {
        objects[idx] =  (CreateableObject *) eiKernel::ObjectFactory::getObject((char *)objectClsid[idx]);
        delete(objectClsid[idx]);
        sz += objects[idx]->deserialize(msg);
    }
    collectionLen = sz;
    return sz;
}




}

