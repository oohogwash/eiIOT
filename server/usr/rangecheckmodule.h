#ifndef RANGECHECKMODULE_H
#define RANGECHECKMODULE_H

#include "eimodule/logicmodule.h"
#include "eiDash/iopoint.h"

class rangeCheckModule : public logicModule
{
    float upper_warning;
    float upper_limit;
    float lower_warning;
    float lower_limit;
public:
    rangeCheckModule();
};

#endif // RANGECHECKMODULE_H
