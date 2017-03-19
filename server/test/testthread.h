#ifndef TESTTHREAD_H
#define TESTTHREAD_H

#if 0

#include <iostream>
using namespace std;
#include "eikernel/thread.h"
#include "eikernel/wqueue.h"


class testthread{
    public:
    testthread()
    {

    }
    void testPubSub();
    int testwq();
};






class WorkItem
{
    string m_message;
    int    m_number;

  public:
    WorkItem(const char* message, int number)
          : m_message(message), m_number(number) {}
    ~WorkItem() {}

    const char* getMessage() { return m_message.c_str(); }
    int getNumber() { return m_number; }
};

class MyThread : public Thread
{
  public:
    void *run() {
        for (int i = 0; i < 5; i++) {
            printf("thread %lu running - %d\n",  (long unsigned int)self(), i+1);
            sleep(2);
        }
        printf("thread done %lu\n", (long unsigned int)self());
        return NULL;
    }
};



class ConsumerThread : public Thread
{
    wqueue<WorkItem*>& m_queue;

  public:
    ConsumerThread(wqueue<WorkItem*>& queue) : m_queue(queue) {}

    void* run() {
        // Remove 1 item at a time and process it. Blocks if no items are
        // available to process.
        for (int i = 0;; i++) {
            printf("thread %lu, loop %d - waiting for item...\n",
                  (long unsigned int)self(), i);
            WorkItem* item = (WorkItem*)m_queue.remove();
            printf("thread %lu, loop %d - got one item\n",
                  (long unsigned int)self(), i);
            printf("thread %lu, loop %d - item: message - %s, number - %d\n",
                  (long unsigned int)self(), i, item->getMessage(),
                   item->getNumber());
            delete item;
        }
        return NULL;
    }
};






class EventConsumer
{
public:
     virtual void onEvent(const int eventID, const void * info) =0;
     virtual void onStateChange(const int fromState, const int toState) =0;
};

class MyEventConsumer : public EventConsumer
{
public:
     void onEvent(const int eventID, const void * info)
    {
      cout <<  "event in myevc" << eventID << (const char *)info<< endl;
    }

     void onStateChange(const int fromState, const int toState)
    {
        cout << fromState << "-->" << toState << "in myevc" <<endl;
    }
};

class Mytest: public EventPublisher<EventConsumer>
{

public:

    void testit()
    {
        cout << eventHandlers[2][0].eventID <<endl ;
        eventHandlers[2][0].handler->onStateChange(7,8);
        eventHandlers[2][0].handler->onEvent(3, "atata0");

        publishStateChange(2, 166, 77,88);
        publishEvent(2,166, "hood");
    }

};

#endif

#endif // TESTTHREAD_H
