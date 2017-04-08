
#include "iomodule.h"
namespace eiModule
{
IoModule::IoModule() : Module(), dir(IODIR::iod_input)
{
    type = IOTYPE::iot_digital;
}

} // eiMsgModule
