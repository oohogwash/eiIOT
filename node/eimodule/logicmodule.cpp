#include "logicmodule.h"

LogicModule::LogicModule():Module()
{
    type = LMTYPE::lmt_connect;
}

void LogicModule::dump()
{
    Module::dump();
}
