#include "testmodule.h"

#include "eiLib/eiLib.h"

#include <iostream>
using namespace std;

#include "eiModule/logicmodule.h"
#include "eiModule/commodule.h"
#include "eiModule/displaymodule.h"
#include "eiModule/iomodule.h"
#include "eiModule/modulemanager.h"


#include "eikernel/execstate.h"




int cmpfunc(const void * a, const void * b)
{
   return ( *(int*)a - *(int*)b );
}

class msgtst
{
public:
    const char *a = "jkflsajfaskl";
    int id = 42;
    const char * b = "hood";

};
ostream& operator<<(ostream& ost, const msgtst& t)
{
        ost<<"|"<<t.a<<'|'<< t.id<<"|"<<t.b<<'|';
        return ost;
}



testmodule::testmodule()
{
    msgtst mt;
      Einfo einfo;

      einfo.Init();

     /// einfo.Run();

      moduleManager mgr;

      comModule cmod;
      strcpy(cmod.id , "comtest");

      mgr.add( &cmod );

      logicModule mod;
      strcpy(mod.id , "test2");

      mgr.add( &mod );

      ioModule mod1;
      strcpy(mod1.id, "test1");

      mgr.add( &mod1 );

      displayModule mod2;
      strcpy(mod2.id, "test");

      mgr.add( &mod2 );

      module  * ptr =  mgr.findModule("test1");


  cout << "Test result " << ptr->id <<endl;

  ptr->dump();

      mgr.dump();

      cout << "delete module ********** " <<endl;

      mgr.remove(&mod);

      mgr.dump();
     cout << "add it back" << endl;
     mgr.add(&mod);
     mgr.dump();

      //einfo.Dump();
      eiCom com;
      com.init();
      eiMsg msg;
    //  const char * tst = "this is a test hshgsfdgsdf";
     // msg.setBody("ABC-42", tst, strlen(tst));
      msg.setBody("bb",&mt, sizeof(mt) );
      com.sendMsg(msg);
      msg.dump();
      eiMsg msgin = com.readMsg();


      msgin.dump();
      msgtst * p = (msgtst *) msgin.body();
      cout << *p << endl;

}
