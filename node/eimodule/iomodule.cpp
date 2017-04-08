
#include "iomodule.h"

IoModule::IoModule() : Module(), dir(IODIR::iod_input)
{
    type = IOTYPE::iot_digital;
}

