
#include "iomodule.h"

ioModule::ioModule() : module(), dir(IODIR::iod_input)
{
    type = IOTYPE::iot_digital;
}

