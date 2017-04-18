#include "logicmodule.h"
#include <string.h>
#include "eiLib/msgdef.h"

using namespace eiCom;


namespace eiModule
{
LogicModule::LogicModule():Module()
{
    type = LMTYPE::lmt_connect;
    strcpy(clsid, "Logic Module");
}

LogicModule::LogicModule( char * id, int16_t type,  int16_t priorityID, int16_t groupID):
    Module(id, LMTYPE::lmt_connect, priorityID, groupID)
{
    strcpy(this->clsid, "Logic Module");

}

unsigned char * LogicModule::serialize(unsigned char * msg)
{
   msg = Module::serialize(msg);
   msg = serSmallString(msg, logic);
   return msg;
}

unsigned char * LogicModule::deserialize( unsigned char * msg)
{
    msg = Module::deserialize(msg);
    msg = deserSmallString(msg, logic);
    return msg;
}




void LogicModule::dump()
{
    Module::dump();
    printf("Logic =; %s]", logic);
}
} // eiMsgModule
