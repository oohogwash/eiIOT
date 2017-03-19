#include "testthread.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>

#if 0

//   testwq();
/*
    MyThread* thread1 = new MyThread();
       MyThread* thread2 = new MyThread();
       thread1->start();
       thread2->start();
       thread1->join();
       thread2->join();
       printf("main done\n");
       exit(0);
*/






void testthread::testPubSub()
{
    Mytest test;

    test.registerHandler( new MyEventConsumer(), 166,2);

    test.testit();

}
int testthread::testwq()
{

    int iterations = 30;

    // Create the queue and consumer (worker) threads
    wqueue<WorkItem*>  queue;
    ConsumerThread* thread1 = new ConsumerThread(queue);
    ConsumerThread* thread2 = new ConsumerThread(queue);
    thread1->start();
    thread2->start();

    // Add items to the queue
    WorkItem* item;
    for (int i = 0; i < iterations; i++) {
        item = new WorkItem("abc", 123);
        queue.add(item);
        item = new WorkItem("def", 456);
        queue.add(item);
        item = new WorkItem("ghi", 789);
        queue.add(item);
        sleep(2);
    }

    // Ctrl-C to end program
    sleep(1);
    printf("Enter Ctrl-C to end the program...\n");
    while (1);
    exit(0);
}

#endif
