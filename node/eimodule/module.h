#ifndef MODULE_H
#define MODULE_H
#include <stdio.h>
#include <inttypes.h>
#include "createableobject.h"

namespace eiModule
{

class Module : public eiKernel::CreateableObject
{
protected:
   int16_t  type=0;
   char idLen;
   int16_t priorityID;
   int16_t groupID;
public:
   Module();
   Module( const char * id, int16_t type,  int16_t priorityID, int16_t groupID);

   static const int MAXIDLEN = 128;

   char id[MAXIDLEN +1];
   void dump();
 //  virtual const char * moduleTypeText() = 0;
   int serialize(unsigned char ** msg);
   int  deserialize(  unsigned char ** msg);


};

} // eiMsgModule

#endif // MODULE_H
