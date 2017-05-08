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
    int serialize(unsigned char ** msg);
    int deserialize ( unsigned char ** msg);

    int serialize (unsigned char ** msg, eiMsg::REST_VERB verb, Module * modules, int moduleLen);



};


}


#endif // MODULEMSG_H
