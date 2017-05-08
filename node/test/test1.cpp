#include "test1.h"
#include "test/util/thread.h"
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

#include <iostream>
using namespace std;
using namespace eiMsg;
using namespace eiCom;
using namespace eiModule;
using namespace eiKernel;



 MsgRecord *  requestResponse( EiCom & com,int msgId, MsgBody & req, int respid, int timeout)
{
    com.sendMsg(msgId, &req);
    int tenms =0;
    do{
       eiMsg::ComIOCP::sleep(10);
       com.processMessages();
       if(com.msgQueue.Size() >0 )
       {
            MsgRecord & rec = com.msgQueue.Dequeue();
           if(atoi(rec.msgid) == respid)
           {
               return & rec;
          }
       }
       tenms+=10;
    }while( tenms <= timeout);
    return 0;
}

unsigned char * processLogon( Logon logon)
{
    // need to set up use pwd checks
    static unsigned char _token[MsgBody::MAXTOKENLEN + 1];
    strcpy((char *)_token, "abcd");
    if(!strcmp(logon.name,"gwhood") && !strcmp(logon.pwd, "xyz") && !strcmp(logon.domain, "EI"))
    {
         return _token;
    }
    return NULL;
}



LogonResponse * requestLogon(EiCom & com, char * user, char * pwd, char * domain)
{
    static LogonResponse lr;
    Logon l(user, pwd, domain);
     MsgRecord * mr = requestResponse(com, mi_Logon, l, mi_LogonResponse, 3000);

    if(mr != NULL)
    {
      //  com.token
        lr.deserialize(mr->getBuffer());
        return &lr;
    }
    return NULL;
}

Test1::Test1()
{

}

void testserSmallCharArr()
{
    unsigned char buff[10240];
    unsigned char * msg = buff;
    int len = serSmallCharArr(&msg, "charvalue", 4);

     char tobuff[10240];
    unsigned char * tomsg = buff;
    unsigned char itemlen;

    int lento = deserSmallCharArr(&tomsg, tobuff, &itemlen);
    int lin = msg - buff;
    int lout = tomsg-buff;
}

void testserSmallString()
{
    unsigned char buff[10240];
    unsigned char * msg = buff;
    int len = serSmallString(&msg, "charvalue");

     char tobuff[10240];
    unsigned char * tomsg = buff;
    unsigned char itemlen;

    int lento = deserSmallString(&tomsg, tobuff, &itemlen);
    int lin = msg - buff;
    int lout = tomsg-buff;
}

void testint16()
{
    unsigned char buff[10240];
    unsigned char * msg = buff;
    int len = serInt16(&msg, 123);

     char tobuff[10240];
    unsigned char * tomsg = buff;
    unsigned char itemlen;
    int16_t item16;
    int lento = deserInt16(&tomsg, &item16);
    int lin = msg - buff;
    int lout = tomsg-buff;
}

void testUChar()
{
    unsigned char buff[10240];
    unsigned char * msg = buff;
    int len = serUChar(&msg, 'H');

     char tobuff[10240];
    unsigned char * tomsg = buff;
    unsigned char itemlen;
    unsigned char uchval;
    int16_t item16;
    int lento = deserUChar(&tomsg, &uchval);

    int lin = msg - buff;
    int lout = tomsg-buff;
}


void Test1::testsendrecv()
{
    testint16();
    testserSmallCharArr();
    testserSmallString();
    testUChar();





}


void Test1::processMessages(EiCom & com)
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
    Response response;
    LoopbackResponse lbr;
    Subscription subscribe;
    Notification notify;
    Get get;
    const unsigned char * token;

   while(com.msgQueue.Size() >0 )
    {

       printf("ddddd\n");
         MsgRecord & rec = com.msgQueue.Dequeue();
         unsigned char ** ptr = rec.getBuffer();
        switch(atoi(rec.msgid))
        {
        case mi_Get:
            get.deserialize(rec.getBuffer());
            printf("Get [%s] [%s]\n", get.getToken(), get.item);
            if(!strcmp(get.item, "ModuleList"))
            {
                strcpy(response.item, "{modules:[{'one'},{'two'}]}");
            }else if (!strcmp(get.item, "Module"))
            {
                char * value = get.getNameValuePair("A1");
                printf("A1 names value is [%s]\n", value);
                strcpy(response.item, "This is module A1");
            }
            else
            {
                strcpy(response.item, "NA");
            }

            com.sendMsg(mi_Response,&response);
            break;
        case mi_Sys:
             p.deserialize(ptr);
            printf("recieved ping %.4s\n", p.token);
            mmin.deserialize(ptr);//(unsigned char *)rec.msgbuffer);
             for( Module ** ptrm = mmin.modules; ptrm - mmin.modules < mmin.modulesLen; ptrm++)
              {
                 (*ptrm)->dump();
                 fflush(stdout);
              }
            break;
        case mi_Logon:
            logon.deserialize(ptr);
            printf("logon ==> [%s] [%s] [%s]\n", logon.name, logon.pwd, logon.domain);
            if( (token = processLogon(logon)) != NULL)
            {
                logonr.setToken(token);
                strcpy((char *)logonr.token, (char *)token);
                com.sendMsg(mi_LogonResponse , &logonr);
            }
            else
            {
                printf("Failed logon usr [%s], pwd [%s], domain [%s]\n", logon.name, logon.pwd, logon.domain);
            }
            break;
        case mi_LogonResponse:
            logonr.deserialize(ptr);
            printf("logon response [%.4s]\n", logonr.token);
            break;
        case mi_Ping:
            p.deserialize(ptr);
            printf("recieved ping %.4s\n", p.token);
            com.sendMsg(mi_PingResponse, &pr);
            break;
        case mi_PingResponse:
            pr.deserialize(ptr);
            printf("recieved ping response\n");
            break;
        case mi_Loopback:
            lb.deserialize(ptr);
            printf("recieved loopback %.4s [%s]\n", lb.token, lb.text);
            strcpy(lbr.text, lb.text);
            com.sendMsg(lbr);
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
            notify.psmsgLen = subscribe.psmsgLen;
            printf("recieved subscription [%s] [%s] [%s]\n",subscribe.topic, subscribe.id, subscribe.psmsg);
            for(int idx = 0;idx<5;idx++)
            {
                com.sendMsg(mi_Notify, &notify);
            }
            break;
        case mi_Notify:
            notify.deserialize(rec.getBuffer());
            printf("recieved notification [%s] [%s] [%s]\n", notify.topic, notify.id, notify.psmsg);

            break;
         case mi_Response:
            response.deserialize(rec.getBuffer());
            printf("response received  [%s]\n", response.item);
            break;
         default:
            printf("undefined msg id [%d] [%c]\n", rec.msgid, rec.msgid);
        }
    }


}


ReadWriteBuffer readStream, writeStream;

class AppA : public Thread
{
    Test1 * tst;
public:
    AppA(Test1 * t) :tst(t){}
   void * run()
   {
       cout << "in appa thread" <<endl;

       ComIOsm  ioserver;
       ioserver.configureStreams(&readStream, &writeStream);
       EiCom server(&ioserver);
       server.init();

       while(1)
       {
           tst->processMessages(server);

           eiMsg::ComIOCP::sleep(1000);
            cout << "AppA" <<endl;

       }
       return 0;
   }
};

class AppB : public Thread
{
    Test1 * tst;
public:
    AppB(Test1 * t) :tst(t){}
    void * run()
    {
        cout << "in appb thread" <<endl;
        ComIOsm ioclient;
        ioclient.configureStreams(&writeStream, &readStream);
        EiCom client(&ioclient);

        while (1)
        {
            tst->processMessages(client);
            eiMsg::ComIOCP::sleep(1000);
            cout << "AppB" <<endl;

            Loopback lb("my loopback");
           client.sendMsg(mi_Loopback, &lb);

           Subscription sub("mytopic", "my ID", "my message", 10);
           client.sendMsg(mi_Subscribe, &sub);
            Ping p;
           client.sendMsg(mi_Ping, &p);

        }
        return 0;
    }
};

void Test1::simplemsg()
{
    AppA * NodeA = new AppA(this);
    AppB * NodeB = new AppB(this);

    NodeA->start();
    NodeB->start();
    NodeA->join();
    NodeB->join();

}





void Test1::subscribe()
{
    /*
     *logon
     * logonresponse
     * get publications
     * subscribe publication
     * ...
     * unsubscribe publication
     *  logoff
     *
     */
}
class Test1Server : public Thread
{
    Test1 * tst;
public:
    Test1Server(Test1 * t) :tst(t){}
   void * run()
   {
       cout << "in Test1Server thread" <<endl;

       ComIOsm  ioserver;
       ioserver.configureStreams(&readStream, &writeStream);
       EiCom server(&ioserver);
       server.init();

       while(1)
       {
           tst->processMessages(server);

           eiMsg::ComIOCP::sleep(1000);
          //  cout << "Test1Server" <<endl;

       }
       return 0;
   }
};






class GetModulesClient : public Thread
{
    Test1 * tst;
public:
    GetModulesClient(Test1 * t) :tst(t){}
    void * run()
    {
        cout << "in GetModulesClient thread" <<endl;
        ComIOsm ioclient;
        ioclient.configureStreams(&writeStream, &readStream);
        EiCom client(&ioclient);


 eiMsg::ComIOCP::sleep(1000);


      LogonResponse *plr = requestLogon(client,"gwhood", "xyz","EI");

        cout << "logon response token [" << plr->getToken() << "]"<<endl;

      eiMsg::Get getModules("ModuleList");

      getModules.setToken(plr->token);

   // client.sendMsg( getModules);

       MsgRecord *  pmr = requestResponse(client, mi_Get, getModules, mi_Response, 1000);
      if(pmr != NULL)
      {
           Response response;
           response.deserialize(pmr->getBuffer());
           printf("response received  [%s]\n", response.item);
           getModules.setItem("Module");
           getModules.setNameValuePair("A1", "input01");
           pmr = requestResponse(client, mi_Get, getModules, mi_Response, 1000);
           if(pmr != NULL)
           {
               response.deserialize(pmr->getBuffer());
                printf("2nd response received  [%s]\n", response.item);
           }

      }







        while (1)
        {
            tst->processMessages(client);
            eiMsg::ComIOCP::sleep(1000);
          //  cout << "AppB" <<endl;




         //  Subscription sub("mytopic", "my ID", "my message", 10);
         //  client.sendMsg(mi_Subscribe, &sub);

        }
        return 0;
    }
};


void Test1::getModules()
{
    /*
     * logon admin
     *  get messages
     *  get modules
     *  get activemodules
     *  get users
     */
    Test1Server * NodeA = new Test1Server(this);
    GetModulesClient * NodeB = new GetModulesClient(this);

    NodeA->start();
    NodeB->start();
    NodeA->join();
    NodeB->join();
}

void Test1::stateMachine()
{
    /*logon user
     * ISA 88 state machine
     * start
     * pause
     * resume
     * stop
     */
    AppA * NodeA = new AppA(this);
    AppB * NodeB = new AppB(this);

    NodeA->start();
    NodeB->start();
    NodeA->join();
    NodeB->join();
}
