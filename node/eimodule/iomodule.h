#ifndef IOMODULE_H
#define IOMODULE_H

#include "module.h"

class IoModule: public Module
{
    //static char ioTypes[] = {"digital", "analog"};
    enum IOTYPE{
        iot_undefined=0,
        iot_digital=10,
        iot_analog
    };

    enum IODIR{
        iod_undefined = 0,
        iod_input,
        iod_output
    };

    IODIR dir;


    char pinID[MAXIDLEN+1];

public:
    IoModule();
    void dump() { Module::dump();}
    const char * moduleTypeText(){return "IO";}


};

#endif // IOMODULE_H
