#include <iostream>
using namespace std;
#include "eiLib/comIOCP.h"
#include "eiLib/eiCom.h"
#include "eiLib/eimsg.h"
#include "eiLib/msgdef.h"

using namespace eiMsg;
using namespace eiCom;

int main(int argc, char *argv[])
{

    Subscribe n((char *)"topic", (char *)"==ID==", (char *)"event",5);

    ComIOCP  io;
    EiCom com(&io);
    com.init();
    printf("read chars\n");
    Logon n1, logon((char *)"userhoodhhhhhhhhhh", (char *)"mypwd");
    EiMsg msg;
    LogonResponse lr;
    Loopback l1, l((char *)"loopbacktest");
    Ping p;
    Publish pub;

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



    return 0;

    while(1)
    {
        com.processMessages();
        ComIOCP::sleep(140);
       while(com.msgQueue.Size() >0 )
        {
            const MsgRecord & rec = com.msgQueue.Dequeue();
            switch(atoi(rec.msgid))
            {
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
