#ifndef COMMODULE_H
#define COMMODULE_H

#include "module.h"

namespace eiModule
{

class ComModule: public Module
{
    enum transportProtocolType {
        tp_undefined = 0,
        tp_shm,
        tp_inprocCall,
        tp_inprocCommon,
        tp_Serial,
        tp_TCP,
        tp_UDP
    };

    enum MessageFormat {
        mf_undefined = 0,
        mf_EI,
        mf_DDS,
        mf_ROS
    };
    enum MessageEncoding{
        me_undefined=0,
        me_text,
        me_text_xml,
        me_text_json,
        me_binary
    };

    enum comType{
        cmt_undefined = 0,
        cmt_EI = 300,
        cmt_ROS
    };
public:
    ComModule();
    ComModule( const char * id, int16_t type,  int16_t priorityID, int16_t groupID);
    void dump() { Module::dump();}
    const char * moduleTypeText(){return "com";}
};

} // eiMsgModule

#endif // COMMODULE_H
