#include "logicmodule.h"

logicModule::logicModule():module()
{
    type = LMTYPE::lmt_connect;
}

void logicModule::dump()
{
    module::dump();
}
