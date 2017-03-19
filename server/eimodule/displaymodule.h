#ifndef DISPLAYMODULE_H
#define DISPLAYMODULE_H

#include "module.h"

class displayModule : public module
{

public:
    displayModule();
    void dump() { module::dump();}
    const char * moduleTypeText(){return "Display";}
};

#endif // DISPLAYMODULE_H
