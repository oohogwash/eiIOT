#include "eiqueue.h"
#include <stdlib.h>
#include "eiLib/eiCom.h"

eiQueue::eiQueue()
{
   front = -1;
   rear = -1;
}

void eiQueue::Enqueue(const char * id, const char * msg, int len)
{
   // Don't allow the queue to grow more
   // than MAX_SIZE - 1
   if ( Size() == MAX_SIZE - 1 )
       //throw new QueueOverFlowException();
{
       return;
   }
   data[rear].msglen = len;
   memcpy(data[rear].msgbuffer, msg, len);
   data[rear].msgbuffer[len]=0;

   printf("---->%d-[%s]\n", len, msg);

   memcpy(data[rear].msgid, id, MSGIDLEN +1);
   data[rear].msgid[MSGIDLEN]=0;

   // MOD is used so that rear indicator
   // can wrap around
   if(++rear >=MAX_SIZE)
       rear = 0;
   //rear = ++rear % MAX_SIZE;
}

msgRecord empty;
const msgRecord & eiQueue::Dequeue()
{
   if ( isEmpty() )
    {
       return  empty;
   }
    //   throw new QueueEmptyException();

   //int ret = data[front];
   int idx = front;

   // MOD is used so that front indicatord
   // can wrap around
   front = ++front % MAX_SIZE;

   return data[idx];
}

const msgRecord & eiQueue::Front()
{
   if ( isEmpty() )
      // throw new QueueEmptyException();
{
       return empty;
   }
   return data[front];
}

int eiQueue::Size()
{
   return abs(rear - front);
}

bool eiQueue::isEmpty()
{
   return ( front == rear ) ? true : false;
}



