#ifndef EICONTROL_H
#define EICONTROL_H
#include "eiCom.h"
namespace eiMsg
{

class EiControl : public EiCom
{
public:
    EiControl();

    void Init();
    void Run();
};
} // eiMsg
#endif // EIMsgCONTROL_H
