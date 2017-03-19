#include <stdlib.h>
#include <stdio.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "testrs232.h"
#include "eiLib/rs232.h"
#include "eiLib/eiCom.h"

testRS232::testRS232()
{

}

void testRS232::testLoopback()
{

}

typedef enum MsgReadState : char
{
    mrs_readSTX,
    mrs_readType,
    mrs_ReadHdrLen,
    mrs_ReadHdr,
    mrs_ReadBody
}mrs;


void testRS232::testRX()
{

      int i, n,
          cport_nr=2,        /* /dev/ttyS0 (COM1 on windows) */
          bdrate=9600;       /* 9600 baud */
      unsigned char buf[4096];
      char mode[]={'8','N','1',0};
      if(RS232_OpenComport(cport_nr, bdrate, mode))
      {
        printf("Can not open comport\n");
        return;
      }

     // RS232_flushRXTX(cport_nr);

      const int MAXMSG=10240;
      char MSG[10240];
      int idx=0;
      char msgType;
      char msgState = mrs_readSTX;
      char hdrlen=0;
      int counter = 0;
      char msgBodyBuffer[MSGLENLEN +1];
      msgBodyBuffer[MSGLENLEN]=0;
      while(1)
      {
        n = RS232_PollComport(cport_nr, buf, 4095);

        if(n > 0)
        {
          buf[n] = 0;   /* always put a "null" at the end of a string! */
          for(i=0; i < n; i++)
          {
              switch(msgState){
              case mrs_readSTX:
                  if(buf[i]== STX)
                  {
                      idx=0;
                      MSG[idx++]= buf[i];
                      msgState = mrs_readType;
                  }
                  else
                  {
                     // printf("ignore %c\n", buf[i]);
                  }
                  break;
              case mrs_readType:
                  if(buf[i] == 72) // check if supported msg type
                  {
                      msgType = MSG[idx++] = buf[i];
                      msgState = mrs_ReadHdrLen;
                  }
                  else
                  {
                      printf("invalud msg %c %d\n", buf[i],buf[i]);
                      // not a supported message so drop out
                      msgState = mrs_readSTX;
                  }
                  break;
              case mrs_ReadHdrLen:
                  hdrlen= buf[i];
                  MSG[idx++]=buf[i];
                  counter = hdrlen -idx; //allow for STX/ msgType etc already read
                  printf("hdr len %d\n", hdrlen);
                  msgState = mrs_ReadHdr;
                  break;
              case mrs_ReadHdr:
                  MSG[idx++]=buf[i];
                  if(--counter <= 0)
                  {
                      memcpy(msgBodyBuffer,&MSG[MSGLENOFFSET], MSGLENLEN);
                      counter = atoi(msgBodyBuffer);
                      printf("msg body len %d\n", counter);
                      msgState = mrs_ReadBody;
                      printf("sequence id = [%c] %d\n", MSG[MSGSEQIDOFFSET], MSG[MSGSEQIDOFFSET]);
                  }
                  break;
              case mrs_ReadBody:
                  MSG[idx++]=buf[i];
                  if(--counter <=0)
                  {
                      MSG[idx]= 0;
                      printf("[%s]%d\n", MSG, idx);
                      msgState = mrs_readSTX;
                   }
                  break;
              }
              /*
              if(msgState == mrs_readType)   //readMsgType == true)
              {
                if(buf[i] == 72) // check if supported msg type
                {
                    MSG[idx++] = buf[i];
                    msgType =  buf[i];
                    printf("Valid msg\n");
                    msgState = mrs_ReadHdrLen;
                }
                else
                {
                    printf("invalud msg %c %d\n", buf[i],buf[i]);
                    inmsg = false; // not a supported message so drop out
                }

                readMsgType = false;
              }else if(inmsg == true)
              {
                  switch(msgType)
                  {
                    case 72:

                  switch(buf[i])
                  {
                    case ETX:
                      inmsg=false;
                      MSG[idx]= 0;
                      printf("[%s]%d\n", MSG, idx);
                      break;
                    case STX:
                      idx=0;
                      inmsg=true;
                      readMsgType = true;
                      break;
                    default:
                      if(inmsg == true)
                      {
                         // printf("{%c}", buf[i]);
                          MSG[idx++] = buf[i];
                      }
                  }



                      break;
                    default:
                      printf("invalid state\n");
                      inmsg = false;
                  }
              }
              else if(buf[i]==STX)
              {
                  idx=0;
                  inmsg=true;
                  readMsgType = true;

 switch(buf[i])
                  {
                    case ETX:
                      inmsg=false;
                      MSG[idx]= 0;
                      printf("[%s]%d\n", MSG, idx);
                      break;
                    case STX:
                      idx=0;
                      inmsg=true;
                      readMsgType = true;
                      break;
                    default:
                      if(inmsg == true)
                      {
                         // printf("{%c}", buf[i]);
                          MSG[idx++] = buf[i];
                      }
                  }

              }
*/
            if(buf[i] < 32)  /* replace unreadable control-codes by dots */
            {
            //  buf[i] = '.';
            }
          }

         // printf("received %i bytes: %s\n", n, (char *)buf);
         // printf("%s", buf);
        }

    #ifdef _WIN32
        Sleep(1000);
    #else
        usleep(100000);  /* sleep for 100 milliSeconds */
    #endif
      }

      return;
    }




void testRS232::testTX()
{


int i=0,
     cport_nr=0,        /* /dev/ttyS0 (COM1 on windows) */
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

 return;
}
