#ifndef DISPLAYMODULE_H
#define DISPLAYMODULE_H

#include "module.h"

namespace eiModule
{

class DisplayModule : public Module
{

public:
    DisplayModule();
    DisplayModule( const char * id, int16_t type,  int16_t priorityID, int16_t groupID);
    void dump() { Module::dump();}
    const char * moduleTypeText(){return "Display";}
    unsigned char *  serialize(unsigned char * msg){return Module::serialize(msg);}
    unsigned char *  deserialize( unsigned char * msg){return Module::deserialize(msg);}

};

}  //eiModule

#endif // DISPLAYMODULE_H
