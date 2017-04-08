#include "logicmodule.h"
namespace eiModule
{
LogicModule::LogicModule():Module()
{
    type = LMTYPE::lmt_connect;
}

void LogicModule::dump()
{
    Module::dump();
}
} // eiMsgModule
