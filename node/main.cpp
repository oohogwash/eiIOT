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

#include "test/test1.h"

using namespace std;

using namespace eiMsg;
using namespace eiCom;
using namespace eiModule;
using namespace eiKernel;


void processMessages(EiCom & com)
{
    const unsigned char * ptr;

    com.processMessages();
    ComIOCP::sleep(140);

    Ping p;
    PingResponse pr;
    ModuleMsg mmin;
    Logon logon;
    LogonResponse logonr;
    Loopback lb;
    LoopbackResponse lbr;
    Subscription subscribe;
    Notification notify;

   while(com.msgQueue.Size() >0 )
    {
       printf("ddddd\n");
         MsgRecord & rec = com.msgQueue.Dequeue();
         unsigned char ** buffer = rec.getBuffer();
        switch(atoi(rec.msgid))
        {
        case mi_Sys:
            p.deserialize(buffer);
            printf("recieved ping %.4s\n", p.token);
            mmin.deserialize(buffer);
             for( Module ** ptrm = mmin.modules; ptrm - mmin.modules < mmin.modulesLen; ptrm++)
              {
                 (*ptrm)->dump();
                 fflush(stdout);
              }
            break;
        case mi_Logon:
            logon.deserialize(rec.getBuffer());
            printf("logon ==> %s %s\n", logon.name, logon.pwd);
            strncpy((char *)logonr.token, "mytoken",6);
            com.sendMsg(mi_LogonResponse , &logonr);
            break;
        case mi_LogonResponse:
            logonr.deserialize(rec.getBuffer());
            printf("logon response [%.4s]\n", logonr.token);
            break;
        case mi_Ping:
            p.deserialize(rec.getBuffer());
            printf("recieved ping %.4s\n", p.token);
            com.sendMsg(mi_PingResponse, &pr);
            break;
        case mi_PingResponse:
            pr.deserialize(rec.getBuffer());
            printf("recieved ping response\n");
            break;
        case mi_Loopback:
            lb.deserialize(rec.getBuffer());
            printf("recieved loopback %.4s [%s]\n", lb.token, lb.text);
            strcpy(lbr.text, lb.text);
            com.sendMsg(mi_LoopbackResponse, &lbr);
            break;
        case mi_LoopbackResponse:
            lbr.deserialize(rec.getBuffer());
            printf("loopback response recieved [%s]",lbr.text);
            break;
        case mi_Subscribe:
            subscribe.deserialize(rec.getBuffer());
            strcpy(notify.topic, subscribe.topic);
            strcpy(notify.id, subscribe.id);
            strcpy(notify.psmsg, subscribe.psmsg);
            printf("recieved subscription [%s]\n",subscribe.topic);
            for(int idx = 0;idx<5;idx++)
            {
                com.sendMsg(mi_Notify, &notify);
            }
            break;
        case mi_Notify:
            notify.deserialize(rec.getBuffer());
            printf("recieved notification [%s] [%s] [%s]\n", notify.topic, notify.id, notify.psmsg);

            break;
        }
    }


}






int main(int argc, char *argv[])
{


unsigned char h[128];
unsigned char *m = h;
int32_t in = 243, out;
    serInt32( &m,&in);
    m=h;
    deserInt32(&m, &out);
    cout << out <<endl;

    int32_t s[] = {22,33,44,55};
    m=h;
    int len = serInt32Arr(&m, s, 3);
    int len1 = m-h;
    int32_t sout[5];
unsigned char sz;
m = h;
    deserInt32Arr(&m, sout, &sz);

    for(int x = 0; x < sz; x++)
    {
        cout << sout[x] << endl;
    }



   // return 0;


    Test1 tst;

  //  tst.testsendrecv();
   // return 0;
    tst.getModules();

    return 0;


    ComIOsm  ioserver;
    ReadWriteBuffer readStream, writeStream;
    ioserver.configureStreams(&readStream, &writeStream);
    ComIOsm ioclient;
    ioclient.configureStreams(&writeStream, &readStream);

    EiCom server(&ioserver);
    server.init();
    printf("read chars\n");
    Logon n1, logon((char *)"userhoodhhhhhhhhhh", (char *)"mypwd", "");
    Loopback l1, l((char *)"loopbacktest");
    Ping p;
    Publication pub;
    EiCom client(&ioclient);


     ModuleMsg mm;
     DisplayModule m1("mod1", 1,2,3);
    ComModule m2("mod1", 1,2,3);
    IoModule m23("mod23", 111,112,113);

     Module  modules[] = {m1,m2,m23};

     unsigned char mmm[1024];
     unsigned char * mmsg1 = mmm;
     p.serialize(&mmsg1);
     mm.serialize(&mmsg1, eiMsg::rv_POST, modules, 3);


      client.sendMsg(mi_Sys, mmm, mmsg1-mmm);

     Loopback lb("my loopback");
    client.sendMsg(mi_Loopback, &lb);

    Subscription sub("mytopic", "my ID", "my message", 10);
    client.sendMsg(mi_Subscribe, &sub);

    client.sendMsg(mi_Ping, &p);

  processMessages(server);
  cout<<"now see what comes back" << endl;
  processMessages(client);
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

   // unsigned char * mmsg = mm.serialize(0,eiMsg::rv_POST, modules, 1);

    ModuleMsg mmin;
  //  mmin.deserialize(mm.collection);


    //  modules = mm;
      for( Module ** ptrm = mmin.modules;
           ptrm - mmin.modules < mmin.modulesLen; ptrm++)
      {
          printf( "-------kkk------%s\n", (*ptrm)->clsName);
          (*ptrm)->dump();
      }

 //cout << "collection len " << mmsg - mm.collection << endl;

    Put s((char *)"sjjkl", (char *)"abcde", 4);


    memcpy(s.token,"1234",4);
    s.tokenLen = 4;


    cout<< "[" << s.item  << s.info << "]" << (int)s.tokenLen <<"}}}"<<endl;



   // cout  << n.topic << "endl" << n.id << "endl"  <<"|" << n.psmsg << "endl" << n.psmsgLen << endl;

    unsigned char msg1[1024];

    unsigned char * ptr = msg1;

    s.serialize(&ptr);
    msg1[ptr-msg1]=0;

    cout<<"----msg0=" << (int)msg1[0] << "[" << msg1 <<"]"<< ptr-msg1<< endl;


    Put s1;
    s1.deserialize(&ptr);

    cout<< "==>[" << s1.item << "|" << s1.infoLen << "|" << s1.info << "]" << s1.token <<endl;

    char test[40];
    unsigned char msg3[1024];
    unsigned char * msg2 = msg3;
    for(int i = 0; i< 5; i++)
    {
        sprintf(test, "test number %d", i);
        serInt32(&msg2, &i);
        serString(&msg2, test);
    }

msg2 = msg3;
 unsigned char * msg2out = msg3;
int32_t x;
    for(int i = 0; i< 5; i++)
    {
         deserInt32(&msg2out, &x);
         deserString(&msg2out, test);
            cout << test <<  "==" << x << endl;
    }

}
