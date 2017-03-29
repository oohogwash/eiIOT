#include <iostream>
using namespace std;
#include "eiLib/eiLib.h"

//#include "test/testmodule.h"
//#include "test/testthread.h"
#include "eiLib/comIOCP.h"
#include "eiLib/eiCom.h"


#include "eiLib/eiqueue.h"

//#include "eiLib/eilogon.h"





int main(int argc, char *argv[])
{
    char _msgBuffer[1024];
    comIOCP  io;
    eiCom com(&io);
 //   RS232_flushRX(2);
    com.init();
  //  com.sendMsg(msg);
    unsigned char buff[2];
    printf("read chars\n");
    int i;

    logon logon;
    eiMsg msg;

    while(1)
    {
       // io.sleep(1000);
       // i=io.read(buff, 1);
         //   printf("%c %d %d ",buff[0], buff[0], i);

        com.processMessages();
        comIOCP::sleep(140);
       while(com.msgQueue.Size() >0 )
        {
            const msgRecord & rec = com.msgQueue.Dequeue();
            switch(atoi(rec.msgid))
            {
                case 5: //logon
                  logon.deserialize((unsigned char *)rec.msgbuffer);
                printf("logon %s %s\n", logon.name, logon.pwd);
                msg.setBody("6", "logon response", 14);
                com.sendMsg(msg);
                break;
            case 7:
                printf("recieved loopback %s", rec.msgbuffer);

            }

           // cout << "record " ;
           // printf("[%s]", rec.msgbuffer);
         //   cout << rec.msgid << "]" << "size " << com.msgQueue.Size();
         //  printf("\n");
        }

    }


    cout << "end of program" << endl;
  return 0;

    //testRS232::testRX();


   // testmodule tm = testmodule();
   // testthread tt = testthread();
   // tt.testPubSub();
  //  eiCom com;
    //  tt.testwq();
//    eiMsg msg;
   // msg.setBody("22", "test body", 9);
    //com.sendMsg( msg);


    eiCom comin;
    eiMsg msgin = comin.readMsg();
    msgin.dump();
    cout << msgin.body() <<endl;
    return 0;
}



