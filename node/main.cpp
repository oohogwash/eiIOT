#include <iostream>
using namespace std;
#include "eiLib/comIOCP.h"
#include "eiLib/eiCom.h"



int main(int argc, char *argv[])
{
    comIOCP  io;
    eiCom com(&io);
    com.init();
    printf("read chars\n");
    logon logon;
    eiMsg msg;
    logonResponse lr;
    loopback l;
    ping p;
    notify n;
    publish pub;

    while(1)
    {
        com.processMessages();
        comIOCP::sleep(140);
       while(com.msgQueue.Size() >0 )
        {
            const msgRecord & rec = com.msgQueue.Dequeue();
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
