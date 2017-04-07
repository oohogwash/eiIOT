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
    logon n1, logon((char *)"userhoodhhhhhhhhhh", (char *)"mypwd");
    eiMsg msg;
    logonResponse lr;
    loopback l1, l((char *)"loopbacktest");
    ping p;
    subscribe n((char *)"topic", (char *)"==ID==", (char *)"event",5);
    publish pub;

    set s((char *)"sjjkl", (char *)"abcde", 4);


    memcpy(s.token,"1234",4);
    s.tokenLen = 4;


    cout<< "[" << s.item << s.itemLen << s.info << "]" << (int)s.tokenLen <<"}}}"<<endl;



   // cout  << n.topic << "endl" << n.id << "endl"  <<"|" << n.psmsg << "endl" << n.psmsgLen << endl;

    unsigned char msg1[1024];

    int len = s.serialize(msg1);
    msg1[len]=0;

    cout<<"----msg0=" << (int)msg1[0] << "[" << msg1 <<"]"<< len<< endl;


    set s1;
    s1.deserialize(msg1);

    cout<< "==>[" << s1.item << "|" << s1.infoLen << "|" << s1.info << "]" << s1.token <<endl;








   // l1.deserialize(msg1);

//    cout  << endl << n1.name << endl << n1.pwd << endl;
    //cout<<l1.text << endl;



    return 0;

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
