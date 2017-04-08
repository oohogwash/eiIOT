#include "module.h"
namespace eiModule
{
Module::Module()
{

}


 void Module::dump()
 {
     printf("Module  type  %s  id = %s\n", moduleTypeText(), id);
 }
} // eiMsgModule
