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

    if (dynamic_cast<ChildPool *>(parent) != nullptr)
        throw runtime_error("parent can't be another ChildPool.");

    auto pointer = new ChildPool(threadCount, parent);
    parent->attachedPools.push_back(unique_ptr<RunnerPool>(pointer));
    pointer->poolMutex = parent->poolMutex;

    return pointer;
}


ChildPool::ChildPool(uint16_t threadCount, RunnerPool *parent) : ChildPool(threadCount, parent, RunnerPool::internal())
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

                            bool empty = this->tasks.empty() && this->parent->tasks.empty();

                            waitCondition.wait(lock, [this, empty] { return this->stop || !empty; });

                            if (this->stop && empty)
                                return;

                            if (!this->tasks.empty())
                            {
                                task = move(this->tasks.front());
                                this->tasks.pop();
                            }
                            else
                            {
                                task = move(parent->tasks.front());
                                parent->tasks.pop();
                            }
                        }
                        task();
                    }
                }
        );
    }
}
