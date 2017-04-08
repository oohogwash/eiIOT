#ifndef DISPLAYMODULE_H
#define DISPLAYMODULE_H

#include "module.h"

namespace eiModule
{

class DisplayModule : public Module
{

public:
    DisplayModule();
    void dump() { Module::dump();}
    const char * moduleTypeText(){return "Display";}
};

}  //eiModule

#endif // DISPLAYMODULE_H
