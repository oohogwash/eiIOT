#include <string.h>
#include "module.h"
#include "msgdef.h"
using namespace eiCom;
namespace eiModule
{
Module::Module()
{
  strcpy(clsName, "Module");
}
Module::Module(const char * id, int16_t type, int16_t priorityID, int16_t groupID) //:CreateableObject("NULL")
{
    this->type = type;
    strcpyn(this->id, MAXIDLEN, id);
    this->priorityID = priorityID;
    this->groupID = groupID;
    strcpy(clsName, "Module");
}

int Module::serialize(unsigned char ** msg)
{
   int sz = serSmallString(msg, id);
   sz += serInt16(msg, type);
   sz += serInt16(msg, priorityID);
   sz += serInt16(msg, groupID);
   return sz;
}

int Module::deserialize(  unsigned char ** msg)
{
    int sz = deserSmallString(msg, id);
    sz += deserInt16(msg, &type);
    sz += deserInt16(msg, &priorityID);
    sz += deserInt16(msg, &groupID);
    return sz;
}


 void Module::dump()
 {
     CreateableObject::dump();
     printf("id = %s, type = %d, priority = %d, group = %d\n", id, type, priorityID, groupID);
 }
} // eiMsgModule
