#include <string.h>
#include "iomodule.h"
namespace eiModule
{
IoModule::IoModule() :  dir(IODIR::iod_input), Module()
{
    type = IOTYPE::iot_digital;
    strcpy(clsName, "IO Module");
}

IoModule::IoModule( const char * id, int16_t type,  int16_t priorityID, int16_t groupID):
    Module(id, 0, priorityID, groupID)
{
    strcpy(this->clsName, "IoModule");

}
unsigned char * IoModule::serialize(unsigned char * msg)
{
   msg = Module::serialize(msg);
 //  msg = serSmallString(msg, logic);
   return msg;
}

unsigned char * IoModule::deserialize( unsigned char * msg)
{
    msg = Module::deserialize(msg);
   // msg = deserSmallString(msg, logic);
    return msg;
}
} // eiMsgModule
