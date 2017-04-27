#ifndef IOMODULE_H
#define IOMODULE_H

#include "module.h"

namespace eiModule
{

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
    IoModule( const char * id, int16_t type,  int16_t priorityID, int16_t groupID);
    void dump() { Module::dump();}
    const char * moduleTypeText(){return "IO";}
    unsigned char *  serialize(unsigned char * msg);
    unsigned char *  deserialize( unsigned char * msg);


};

} // eiMsgModule

#endif // IOMODULE_H
