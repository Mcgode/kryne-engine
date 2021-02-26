/**
 * @file
 * @author Max Godefroy
 * @date 25/02/2021.
 */

#include "kryne-engine/Dispatch/ChildPool.h"


ChildPool *ChildPool::make(RunnerPool *parent, uint16_t threadCount)
{
    if (parent == nullptr)
        throw runtime_error("parent can't be null");

    auto pointer = new ChildPool(threadCount, parent);
    parent->attachedPools.push_back(unique_ptr<ChildPool>(pointer));

    return pointer;
}


ChildPool::ChildPool(uint16_t threadCount, RunnerPool *parent) : BasePool(threadCount, parent->poolMutex, parent->waitCondition, RunnerPool::internal())
{
    this->parent = parent;

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

                            waitCondition->wait(lock, [this] { return this->stop || !this->tasks.empty() || !this->parent->tasks.empty(); });

                            if (this->stop && this->tasks.empty() && this->parent->tasks.empty())
                                return;

                            if (!this->tasks.empty())
                            {
                                task = move(this->tasks.front());
                                this->tasks.pop();
                            }
                            else
                            {
                                task = move(this->parent->tasks.front());
                                this->parent->tasks.pop();
                            }
                        }
                        task();
                    }
                }
        );
    }
}
