#include <string.h>
#include "commodule.h"

namespace eiModule
{


ComModule::ComModule() : Module()
{
    type = comType::cmt_EI;
    strcpy(clsName, "ComModule");
}
ComModule::ComModule( const char * id, int16_t type,  int16_t priorityID, int16_t groupID):
    Module(id, 0, priorityID, groupID)
{
    strcpy(this->clsName, "ComModule");

}

} // eiMsgModule
