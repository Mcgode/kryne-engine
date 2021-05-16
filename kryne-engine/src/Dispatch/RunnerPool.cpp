/**
 * @file
 * @author Max Godefroy
 * @date 25/02/2021.
 */

#include "kryne-engine/Dispatch/RunnerPool.h"


RunnerPool::RunnerPool(uint16_t threadCount): ThreadPool(threadCount, &this->_poolMutex, &this->_waitCondition, ThreadPool::internal())
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

                        waitCondition->wait(lock, [this] { return this->stop || !this->tasks.empty(); });

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
    // Call ahead of ThreadPool destructor, to prevent local conditional variable lock
    this->destroyPool();
}
