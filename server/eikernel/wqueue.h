#ifndef WQUEUE_H
#define WQUEUE_H

#if 0

#ifndef null
#define null 0
#endif

template<class T>
class EventPublisher
{
    struct _eventHandlerMap
    {
        int eventID;
        T * handler;
    };
    static const int MAXEVENTHANDLERS = 20;
    static const int MAXEVENTCLASSES=5;
    int numEventHandlers[MAXEVENTCLASSES];

protected:
    _eventHandlerMap eventHandlers [MAXEVENTCLASSES+1][MAXEVENTHANDLERS+1];

public:
        EventPublisher()
        {
            for(int idx = 0; idx < MAXEVENTCLASSES;idx++)
            {
                numEventHandlers[idx] = 0;
                for(int i = 0 ; i < MAXEVENTHANDLERS;i++)
                {
                    eventHandlers[idx][i].eventID= 0;
                    eventHandlers[idx][i].handler = null;
                }
            }
        }

        int publishEvent(const int eventClassID, const int eventID, const void * info)
        {
            if( eventClassID < MAXEVENTCLASSES)
            {
                for(int idx = 0; idx < numEventHandlers[eventClassID]; idx++)
                {
                    if(eventHandlers[eventClassID][idx].eventID == eventID)
                    {
                        eventHandlers[eventClassID][idx].handler->onEvent(eventID, info);
                    }
                }
            }
            else
            {
                return -1;
            }
        }
        int publishStateChange(const int eventClassID, const int eventID, const int fromState, const int toState)
        {
            if( eventClassID < MAXEVENTCLASSES)
            {
                for(int idx = 0; idx < numEventHandlers[eventClassID]; idx++)
                {
                    if(eventHandlers[eventClassID][idx].eventID == eventID)
                    {
                        eventHandlers[eventClassID][idx].handler->onStateChange(fromState, toState);
                    }
                }
            }
            else
            {
                return -1;
            }
        }


        int registerHandler (T * handler, int eventID, int eventClassID)
        {
            if(numEventHandlers[eventClassID] < MAXEVENTHANDLERS)
            {
                eventHandlers[eventClassID][numEventHandlers[eventClassID]].eventID = eventID;
                eventHandlers[eventClassID][numEventHandlers[eventClassID]].handler = handler;
               return numEventHandlers[eventClassID]++;
            }
            return 0;
        }
        int unregisterHandler (int eventHandlerIdx, int eventClassID)
        {
            for(int idx = eventHandlerIdx ; idx < numEventHandlers[eventClassID]; idx++)
            {
                eventHandlers[eventClassID][idx] = eventHandlers[eventClassID][idx+1];
            }
            return --numEventHandlers[eventClassID];
        }
};





#include <pthread.h>
#include <list>

using namespace std;

template <typename T> class wqueue
{
    list<T>   m_queue;
    pthread_mutex_t m_mutex;
    pthread_cond_t  m_condv;

public:
    wqueue() {
        pthread_mutex_init(&m_mutex, NULL);
        pthread_cond_init(&m_condv, NULL);
    }
    ~wqueue() {
        pthread_mutex_destroy(&m_mutex);
        pthread_cond_destroy(&m_condv);
    }
    void add(T item) {
        pthread_mutex_lock(&m_mutex);
        m_queue.push_back(item);
        pthread_cond_signal(&m_condv);
        pthread_mutex_unlock(&m_mutex);
    }
    T remove() {
        pthread_mutex_lock(&m_mutex);
        while (m_queue.size() == 0) {
            pthread_cond_wait(&m_condv, &m_mutex);
        }
        T item = m_queue.front();
        m_queue.pop_front();
        pthread_mutex_unlock(&m_mutex);
        return item;
    }
    int size() {
        pthread_mutex_lock(&m_mutex);
        int size = m_queue.size();
        pthread_mutex_unlock(&m_mutex);
        return size;
    }
};


#endif


#endif // WQUEUE_H
