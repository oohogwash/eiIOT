#ifndef MODULEMSG_H
#define MODULEMSG_H

#include "eimsg.h"
#include "module.h"


namespace eiModule
{

class ModuleMsg : public eiMsg::Rest
{
    eiMsg::REST_VERB verb;

public:
    Module ** modules ;
    int16_t modulesLen;

    ModuleMsg();
    unsigned char * serialize (unsigned char * msg, eiMsg::REST_VERB verb, Module * modules, int moduleLen);
    unsigned char * deserialize (unsigned char * msg);


};


}


#endif // MODULEMSG_H
