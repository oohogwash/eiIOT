#ifndef MODULE_H
#define MODULE_H
#include <stdio.h>

namespace eiModule
{

class Module
{

public:
   int  type=0;

   int priorityID;
   int groupID;


public:
   static const int MAXIDLEN = 128;

   char id[MAXIDLEN +1];
   void dump();
   virtual const char * moduleTypeText() = 0;
   Module();

};

} // eiMsgModule

#endif // MODULE_H
