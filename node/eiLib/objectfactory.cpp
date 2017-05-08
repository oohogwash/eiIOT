#include "objectfactory.h"
#include <string.h>
#include "commodule.h"
#include "logicModule.h"
#include "displaymodule.h"
#include "iomodule.h"
#include "msgdef.h"

using namespace eiModule;





typedef enum objId : int
{
    oi_string,
    oi_int,
    oi_float,
    oi_logicModule,
    oi_displayModule,
    oi_ioModule,
    oi_comModule
}objId;

static  const char * objName [] = {
    "string",
    "int",
    "float",
  "LogicModule",
  "DisplayModule",
  "IoModule",
   "ComModule",

   0
};

int findObject( const char * id)
{
    for(int idx = 0; objName[idx] != 0; idx++ )
    {
        if(!strcmp(objName[idx], id))
        {
            return idx+1;
        }
    }
    printf("ERROR Cannot find object id %s\n", id);
    return 0;
}


namespace eiKernel
{

ObjectFactory::ObjectFactory()
{

}

void * ObjectFactory::getObject( const char * id)
{
    switch(findObject(id))
    {
    case oi_int:
        return new int;
    case oi_string:
        return new char *;
    case oi_float:
        return new float;
    case oi_displayModule:
        return new DisplayModule();
    case oi_ioModule:
        return new IoModule();
    case oi_logicModule:
        return new LogicModule();
    case oi_comModule:
        return new ComModule();
    default:
        printf("ERROR: No constructor for  ID %s\n", id);

    }

    return 0;
}


}
