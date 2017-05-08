#ifndef EIQUEUE_H
#define EIQUEUE_H

#include "msgrecord.h"

namespace eiMsg
{

class EiQueue
{
    static const int MAX_SIZE = 55;
private:
   MsgRecord data[MAX_SIZE];
   int front;
   int rear;
public:
   EiQueue();
   void Enqueue(const char * id, const unsigned char * msg, int len);
   MsgRecord & Dequeue();
   MsgRecord & Front();
   int Size();
   bool isEmpty();
};

} // eiMsg
#endif // EIMsgQUEUE_H
