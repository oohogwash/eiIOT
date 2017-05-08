#include "logicmodule.h"
#include <string.h>
#include "msgdef.h"

using namespace eiCom;


namespace eiModule
{
LogicModule::LogicModule():Module()
{
    type = LMTYPE::lmt_connect;
    strcpy(clsName, "LogicModule");
}

LogicModule::LogicModule( const char * id, int16_t type,  int16_t priorityID, int16_t groupID):
    Module(id, LMTYPE::lmt_connect, priorityID, groupID)
{
    strcpy(this->clsName, "LogicModule");

}

int LogicModule::serialize(unsigned char ** msg)
{
   int sz = Module::serialize(msg);
   sz += serSmallString(msg, logic);
   return sz;
}

int LogicModule::deserialize( unsigned char ** msg)
{
    int sz = Module::deserialize(msg);
    sz += deserSmallString(msg, logic);
    return sz;
}




void LogicModule::dump()
{
    Module::dump();
    printf("Logic =; %s]", logic);
}
} // eiMsgModule
