#ifndef MODULE_H
#define MODULE_H
#include <stdio.h>

namespace eiModule
{

class Module
{

public:
   int  type=0;
   char idLen;
   int priorityID;
   int groupID;


public:
   static const int MAXIDLEN = 128;

   char id[MAXIDLEN +1];
   void dump();
   virtual const char * moduleTypeText() = 0;
   Module();
   virtual int serialize(unsigned char * msg);
   virtual int deserialize( unsigned char * msg);


};

} // eiMsgModule

#endif // MODULE_H
