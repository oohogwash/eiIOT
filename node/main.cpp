#include <iostream>

using namespace std;
#include "comIOCP.h"
#include "eiCom.h"
#include "eimsg.h"
#include  "msgdef.h"
#include  "modulemsg.h"
#include  "objectfactory.h"
#include  "LogicModule.h"
#include "commodule.h"
#include  "iomodule.h"
#include "displaymodule.h"

using namespace std;

using namespace eiMsg;
using namespace eiCom;
using namespace eiModule;
using namespace eiKernel;

char * itoa(int i, char buff[])
{
    sprintf(buff, "%d", i);
    return buff;
}

int main(int argc, char *argv[])
{

    Subscribe n((char *)"topic", (char *)"==ID==", (char *)"event",5);

    ComIOsm  io;
    EiCom com(&io);
    com.init();
    printf("read chars\n");
    Logon n1, logon((char *)"userhoodhhhhhhhhhh", (char *)"mypwd");
    EiMsg msg;
    LogonResponse lr;
    Loopback l1, l((char *)"loopbacktest");
    Ping p;
    Publish pub;
    EiCom send(&io);
    EiMsg sm;
     char buff[20];

     ModuleMsg mm;
     DisplayModule m1("mod1", 1,2,3);
    ComModule m2("mod1", 1,2,3);
    IoModule m23("mod23", 111,112,113);

     Module  modules[] = {m1,m2,m23};

     unsigned char mmm[1024];
  //   unsigned char * mmsg = mm.serialize(mmm, eiMsg::rv_POST, modules, 3);

     unsigned char * mmsg1 = mmm;
      mmsg1 = p.serialize(mmsg1);
      mmsg1 =       mm.serialize(mmsg1, eiMsg::rv_POST, modules, 3);



    sm.setBody(itoa(mi_Sys, buff), mmm,mmsg1-mmm);
    send.sendMsg(sm);
      send.sendMsg(sm);
      sm.setBody(itoa(mi_Logon,buff), &logon);
      send.sendMsg(sm);

  ModuleMsg mmin;
  unsigned char * ptr;
  //  while(1)
    {
        com.processMessages();
        ComIOCP::sleep(140);
       while(com.msgQueue.Size() >0 )
        {
           printf("ddddd\n");
            const MsgRecord & rec = com.msgQueue.Dequeue();
            switch(atoi(rec.msgid))
            {
            case mi_Sys:
                ptr = p.deserialize((unsigned char *)rec.msgbuffer);
                printf("recieved ping %.4s\n", p.token);
                mmin.deserialize(ptr);//(unsigned char *)rec.msgbuffer);
                 for( Module ** ptrm = mmin.modules; ptrm - mmin.modules < mmin.modulesLen; ptrm++)
                  {
                     (*ptrm)->dump();
                     fflush(stdout);
                  }
                break;
            case mi_Logon:
                logon.deserialize((unsigned char *)rec.msgbuffer);
                printf("logon ==> %s %s\n", logon.name, logon.pwd);
                msg.setBody("6", "logon response", 14);
                com.sendMsg(msg);
                break;
            case mi_Ping:
                p.deserialize((unsigned char *)rec.msgbuffer);
                printf("recieved ping %.4s\n", p.token);
                break;
            case mi_Loopback:
                l.deserialize((unsigned char *)rec.msgbuffer);
                printf("recieved loopback %.4s\n", l.token);
                break;
            case mi_LogonResponse:
                lr.deserialize((unsigned char *)rec.msgbuffer);
                printf("logon response %.4s\n", lr.token);
                break;
            case mi_Notify:

                break;
            }
        }

    }


    cout << "end of program" << endl;


  return 0;

}


void tst()
{

    ObjectFactory of;

    void * obj = of.getObject("Logic Module");

   Module *pm = (Module *) obj;
   pm->dump();
cout << "==========" << endl;

    ModuleMsg mm;
   LogicModule m1("mod1", 1,2,3);
   ComModule m2("mod2", 11,12,13);
   DisplayModule m23("mod23", 111,112,113);

    Module modules[] = {m1,m2,m23};

    unsigned char * mmsg = mm.serialize(0,eiMsg::rv_POST, modules, 1);

    ModuleMsg mmin;
    mmin.deserialize(mm.collection);


    //  modules = mm;
      for( Module ** ptrm = mmin.modules;
           ptrm - mmin.modules < mmin.modulesLen; ptrm++)
      {
          printf( "-------kkk------%s\n", (*ptrm)->clsName);
          (*ptrm)->dump();
      }

 cout << "collection len " << mmsg - mm.collection << endl;

    Put s((char *)"sjjkl", (char *)"abcde", 4);


    memcpy(s.token,"1234",4);
    s.tokenLen = 4;


    cout<< "[" << s.item << s.itemLen << s.info << "]" << (int)s.tokenLen <<"}}}"<<endl;



   // cout  << n.topic << "endl" << n.id << "endl"  <<"|" << n.psmsg << "endl" << n.psmsgLen << endl;

    unsigned char msg1[1024];

    unsigned char * ptr = s.serialize(msg1);
    msg1[ptr-msg1]=0;

    cout<<"----msg0=" << (int)msg1[0] << "[" << msg1 <<"]"<< ptr-msg1<< endl;


    Put s1;
    s1.deserialize(msg1);

    cout<< "==>[" << s1.item << "|" << s1.infoLen << "|" << s1.info << "]" << s1.token <<endl;

    char test[40];
    unsigned char msg3[1024];
    unsigned char * msg2 = msg3;
    for(int i = 0; i< 5; i++)
    {
        sprintf(test, "test number %d", i);
        msg2 = serInt32(msg2, i);
        msg2 = serString(msg2, test);
    }

msg2 = msg3;
int32_t x;
    for(int i = 0; i< 5; i++)
    {
         msg2 = deserInt32(msg2, &x);
         msg2 = deserString(msg2, test);
            cout << test <<  "==" << x << endl;
    }

   // l1.deserialize(msg1);

//    cout  << endl << n1.name << endl << n1.pwd << endl;
    //cout<<l1.text << endl;








}
