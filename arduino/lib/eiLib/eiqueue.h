#ifndef EIQUEUE_H
#define EIQUEUE_H

#include "msgrecord.h"

class eiQueue
{
    static const int MAX_SIZE = 55;
private:
   msgRecord data[MAX_SIZE];
   int front;
   int rear;
public:
   eiQueue();
   void Enqueue(const char * id, const char * msg, int len);
   const msgRecord & Dequeue();
   const msgRecord & Front();
   int Size();
   bool isEmpty();
};


#endif // EIQUEUE_H
