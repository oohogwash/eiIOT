#ifndef DISPLAYMODULE_H
#define DISPLAYMODULE_H

#include "module.h"

class DisplayModule : public Module
{

public:
    DisplayModule();
    void dump() { Module::dump();}
    const char * moduleTypeText(){return "Display";}
};

#endif // DISPLAYMODULE_H
