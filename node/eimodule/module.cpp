#include "module.h"
#include "eiLib/msgdef.h"
using namespace eiCom;
namespace eiModule
{
Module::Module()
{

}

unsigned char * serInt( unsigned char * msg, int value)
{
    SCH ich;
    ich.int16 = value;
    *msg++ = 2;
    memcpyn2(msg, ich.ch,2 );
    msg+=2;
    return msg;

}

int Module::serialize(unsigned char * msg)
{
    int idx=0;
    msg[idx++] = idLen;
    memcpyn((char *)&msg[idx], MAXIDLEN, (char *)id, idLen );
    idx+=idLen;
    SCH ich;
    memcpyn2(&msg[idx], ich.ch, 2 );



    int  type=0;
    char idLen;
    int priorityID;
    int groupID;

    return idLen + 1;
}

int Module::deserialize( unsigned char * msg)
{
    idLen =  msg[0];
    memcpyn((char *)id, MAXIDLEN, (char *)&msg[1], idLen,true);
    return idLen+1;
}


 void Module::dump()
 {
     printf("Module  type  %s  id = %s\n", moduleTypeText(), id);
 }
} // eiMsgModule
