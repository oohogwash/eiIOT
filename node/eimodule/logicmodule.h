#ifndef LOGICMODULE_H
#define LOGICMODULE_H

#include "module.h"

namespace eiModule
{

class LogicModule : public Module
{
    enum LMTYPE{
        lmt_connect=100,
        lmt_map
    };

    static const int MAXLOGICLEN = 256;
    char logic[MAXLOGICLEN];
public:
    LogicModule();
    LogicModule( const char * id, int16_t type,  int16_t priorityID, int16_t groupID);
    void dump();
    const char * moduleTypeText(){return "logic";}
    int  serialize(unsigned char ** msg);
    int  deserialize(  unsigned char ** msg);

};

}// eiMsgModule

#endif // LOGICMODULE_H
