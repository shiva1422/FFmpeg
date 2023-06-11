//
// Created by shivaaz on 9/7/22.
//

#ifndef KALASOFT_SAFEQUEUE_H
#define KALASOFT_SAFEQUEUE_H

#include <queue>
#include <iterator>
#include <mutex>

template<class T> class SafeQueue{

public:

    SafeQueue(){}
    //ToDO &?
    void push(const T &t)
    {
        qLock.lock();
        q.push(t);
        qLock.unlock();
    }

    /*
     * better deque after size check;
     * TODO std::optional<T> deque();c++17
    */
    T pop()
    {
        T t = 0;
        qLock.lock();
        if(q.size())
        {
            //TODO move is better?
            t = q.front();
            q.pop();
        }
        qLock.unlock();
        //else why return defaulted setflag instead?
        return t;
    }

    KSFORCEINLINE T popUnsafe()
    {
        T t = q.front();
        q.pop();
        return t;
    }

    size_t size()
    {
        size_t size1 = 0;
        qLock.lock();
        size1 = q.size();
        qLock.unlock();
        return size1;
    }

    KSFORCEINLINE size_t sizeUnsafe()
    {
        return q.size();
    }

    /*
     * To do q operations in bunch at once directly instead of locking every time like deque multiple elements(popUnsafe).Will block others,so call safeZoneEnd() as fast as possible
     * inside safe zone always use unsafe methods as RECURSIVE MUTEX is not  used
     */
    KSFORCEINLINE void safeZoneStart()
    {
        qLock.lock();
    }

    //only followed by safeZoneStart().
    KSFORCEINLINE void safeZoneEnd()
    {
        //TODO if exception or unusual this may not be good instead lockguard or unique lock would be better to safeZoneStart()
        qLock.unlock();
    }

    //TODO move?
protected:

    std::queue<T> q;

    std::mutex qLock;
};

//TODO check need move to c pthreads,moveing etc consider
#endif //KALASOFT_SAFEQUEUE_H
