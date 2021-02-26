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
    {
        if (this->threads[i].joinable())
            this->threads[i].join();
    }

    delete[] this->threads;
}


void BasePool::swapQueues(queue<function<void()>> &swapQueue, bool allowNonEmpty)
{
    unique_lock<mutex> lock(*this->poolMutex);

    if (!allowNonEmpty && !this->tasks.empty())
        throw runtime_error("Can't swap non-empty task queue");

    swap(swapQueue, this->tasks);
}
