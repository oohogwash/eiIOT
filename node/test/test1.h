#ifndef TEST1_H
#define TEST1_H
#include "eiCom.h"

class Test1
{
public:
    Test1();
    void subscribe();
    void stateMachine();
    void getModules();
    void simplemsg();
    void processMessages(eiMsg::EiCom & com);
    void testsendrecv();
};

#endif // TEST1_H
