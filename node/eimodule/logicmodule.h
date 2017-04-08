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
    void dump();
    const char * moduleTypeText(){return "logic";}
};

}// eiMsgModule

#endif // LOGICMODULE_H
