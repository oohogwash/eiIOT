#ifndef EIQUEUE_H
#define EIQUEUE_H

#include "eiLib/msgrecord.h"

namespace eiMsg
{

class EiQueue
{
    static const int MAX_SIZE = 105;
private:
   MsgRecord data[MAX_SIZE];
   int front;
   int rear;
public:
   EiQueue();
   void Enqueue(const char * id, const char * msg, int len);
   const MsgRecord & Dequeue();
   const MsgRecord & Front();
   int Size();
   bool isEmpty();
};

} // eiMsg
#endif // EIMsgQUEUE_H
