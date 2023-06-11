//
// Created by kalasoft on 1/9/22.
//

#ifndef KALASOFT_BLOCKINGQUEUE_H
#define KALASOFT_BLOCKINGQUEUE_H

#include "stdlib.h"
#include "queue"
#include "mutex"
#include "Logger.h"

//TODO implement like STL:std::queue;
template<class T>
class BlockingQueue {

public:
    BlockingQueue();
    BlockingQueue(int size);
    void enque(const T obj);
    T deque();
    int size();
protected:
    virtual void onQueued(){};//just after adding data in safeZone
    virtual void onDequed(){};//just after removing data in safeZone;

protected:
    int maxCnt = 5;
    std::queue<T> q;//TODO
    std::mutex qMutex;
    std::condition_variable qCond;//separate for full/empty?no cuz using q.size()? maintain count;

};


template<class T>
BlockingQueue<T>::BlockingQueue() {
}

template<class T>
BlockingQueue<T>::BlockingQueue(int size) : maxCnt(size){

}

template<class T>
void BlockingQueue<T>::enque(const T obj) {

    std::unique_lock<std::mutex> lock(qMutex);
    qCond.wait(lock,[&]{
        KSLog::debug("BlockingQue","side max = %d and q size %d",maxCnt,q.size());
        return maxCnt != q.size();
    });
    q.push(obj);

#ifndef NDEBUG
   if(q.size() == maxCnt)
    {
       //KSLog::warn("BlockingQ","full");
    }
#endif

    qCond.notify_all();

}

template<class T>
T BlockingQueue<T>::deque() {

    //why block when not needed

    std::unique_lock<std::mutex> lock(qMutex);
    qCond.wait(lock,[this]{
        return !q.empty();//use count instead of size()?
    });

    T val(std::move(q.front()));
    q.pop();
    qCond.notify_all();
    return val;

}

template<class T>
int BlockingQueue<T>::size() {
    std::lock_guard<std::mutex> lock(qMutex);
    return q.size();//maintain count?
}




#endif //KALASOFT_BLOCKINGQUEUE_H
