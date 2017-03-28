#include <iostream>
using namespace std;
#include "eiLib/eiLib.h"

//#include "test/testmodule.h"
//#include "test/testthread.h"
#include "eiLib/comIOCP.h"
#include "eiLib/eiCom.h"
#include "test/testrs232.h"

#include "eiLib/rs232.h"
//#include "test/testrs232.h"




void testRS232()
{
    int i=0,
         cport_nr=3,        /* /dev/ttyS0 (COM1 on windows) */
         bdrate=9600;       /* 9600 baud */

     char mode[]={'8','N','1',0},
          str[2][512];


     strcpy(str[0], "The quick brown fox jumped over the lazy grey dog.\n");

     strcpy(str[1], "Happy serial programming!\n");

     if(RS232_OpenComport(cport_nr, bdrate, mode))
     {
       printf("Can not open comport\n");

       return;
     }


     while(1)
     {
       RS232_cputs(cport_nr, str[i]);

       printf("sent: %s\n", str[i]);

   #ifdef _WIN32
       Sleep(1000);
   #else
       usleep(1000000);  /* sleep for 1 Second */
   #endif

       i++;

       i %= 2;
     }


}

#include "eiLib/eiqueue.h"



int main(int argc, char *argv[])
{
   // testQ();
   // return 0;
    //testRS232::testRX();

char _msgBuffer[1024];

  //  eiMsg msg;


   //  int length = msg.setBody("22", "hoodabcdefgh", 9);
   //  memcpy(_msgBuffer, msg.msg(), length);
    //  _msgBuffer[length] = 0;



   // MsgRecordMgr recmgr;
    comIOCP  io;
    eiCom com(&io);
    RS232_flushRX(2);
    com.init();
  //  com.sendMsg(msg);
    unsigned char buff[2];
    printf("read chars\n");
    int i;
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
           // cout << "record " ;
           // printf("[%s]", rec.msgbuffer);
         //   cout << rec.msgid << "]" << "size " << com.msgQueue.Size();
           printf("\n");
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



