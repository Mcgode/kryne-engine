/**
 * @file
 * @author Max Godefroy
 * @date 25/02/2021.
 */

#include "kryne-engine/Dispatch/RunnerPool.h"


RunnerPool::RunnerPool(uint16_t threadCount, enum internal): threadCount(threadCount), poolMutex(&this->_poolMutex)
{
    this->threads = new thread[this->threadCount];
}


RunnerPool::RunnerPool(uint16_t threadCount): RunnerPool(threadCount, RunnerPool::internal())
{
    for (uint16_t i = 0; i < this->threadCount; i++)
    {
        threads[i] = thread(
            [this]
            {
                for (;;)
                {
                    function<void()> task;
                    {
                        unique_lock<mutex> lock(*this->poolMutex);

                        waitCondition.wait(lock, [this] { return this->stop || !this->tasks.empty(); });

                        if (this->stop && this->tasks.empty())
                            return;

                        task = move(this->tasks.front());
                        this->tasks.pop();
                    }
                    task();
                }
            }
        );
    }
}


RunnerPool::~RunnerPool()
{
    {
        unique_lock<mutex> lock(*this->poolMutex);
        this->stop = true;
    }

    this->waitCondition.notify_all();

    for (uint16_t i = 0; i < this->threadCount; i++)
        this->threads[i].join();

    delete[] this->threads;
}
