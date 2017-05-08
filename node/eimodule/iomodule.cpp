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
int IoModule::serialize(unsigned char ** msg)
{
   return  Module::serialize(msg);
}

int IoModule::deserialize( unsigned char ** msg)
{
    return Module::deserialize(msg);

}
} // eiMsgModule
