#include "objectfactory.h"
#include <string.h>

#include "logicModule.h"
#include "displaymodule.h"
#include "iomodule.h"

using namespace eiModule;

typedef enum objId : int
{
    oi_logicModule = 1,
    oi_displayModule,
    oi_ioModule
}objId;

static  const char * objName [] = {
  "Logic Module",
  "Display Module",
  "IO Module",
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
    case oi_displayModule:
        return new DisplayModule();
    case oi_ioModule:
        return new IoModule();
    case oi_logicModule:
        return new LogicModule();
    }


    return 0;
}


}
