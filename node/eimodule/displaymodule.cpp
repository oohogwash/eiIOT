#include <string.h>
#include "displaymodule.h"
namespace eiModule
{
DisplayModule::DisplayModule() : Module()
{
    type = 200;
    strcpy(clsName, "DisplayModule");
}
DisplayModule::DisplayModule( const char * id, int16_t type,  int16_t priorityID, int16_t groupID):
    Module(id, 0, priorityID, groupID)
{
    strcpy(this->clsName, "DisplayModule");

}
} // eiMsgModule
