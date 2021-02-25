/**
 * @file
 * @author Max Godefroy
 * @date 25/02/2021.
 */

#include "kryne-engine/Dispatch/BasePool.h"

BasePool::BasePool(uint16_t threadCount, mutex *mutex, condition_variable *condition, BasePool::internal)
    : threadCount(threadCount),
      poolMutex(mutex),
      waitCondition(condition)
{
    this->threads = new thread[this->threadCount];
}


BasePool::~BasePool()
{
    {
        unique_lock<mutex> lock(*this->poolMutex);
        this->stop = true;
    }

    this->waitCondition->notify_all();

    for (uint16_t i = 0; i < this->threadCount; i++)
        this->threads[i].join();

    delete[] this->threads;
}
