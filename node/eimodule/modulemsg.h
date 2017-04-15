#ifndef MODULEMSG_H
#define MODULEMSG_H

#include "eiLib/eimsg.h"
#include "eimodule/module.h"
#

namespace eiModule
{

class ModuleMsg : public eiMsg::Rest
{
public:
    ModuleMsg();
    ModuleMsg(eiMsg::REST_VERB verb, Module * modules [], int moduleLen);
};


}


#endif // MODULEMSG_H
