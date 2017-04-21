#include "module.h"
#include "msgdef.h"
using namespace eiCom;
namespace eiModule
{
Module::Module()
{

}
Module::Module(char * id, int16_t type, int16_t priorityID, int16_t groupID) //:CreateableObject("NULL")
{
    this->type = type;
    strcpyn(this->id, MAXIDLEN, id);
    this->priorityID = priorityID;
    this->groupID = groupID;
}

unsigned char * Module::serialize(unsigned char * msg)
{
   msg = serSmallString(msg, id);
   msg = serInt16(msg, type);
   msg = serInt16(msg, priorityID);
   msg = serInt16(msg, groupID);
   return msg;
}

unsigned char * Module::deserialize( unsigned char * msg)
{
    msg = deserSmallString(msg, id);
    msg = deserInt16(msg, &type);
    msg = deserInt16(msg, &priorityID);
    msg = deserInt16(msg, &groupID);
    return msg;
}


 void Module::dump()
 {
    // printf("Module  type  %s  id = %s\n", moduleTypeText(), id);
     printf(" id = %s, type = %d, priotiry = %d, group = %d\n", id, type, priorityID, groupID);
 }
} // eiMsgModule
