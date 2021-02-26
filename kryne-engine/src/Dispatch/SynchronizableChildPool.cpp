/**
 * @file
 * @author Max Godefroy
 * @date 25/02/2021.
 */

#include "kryne-engine/Dispatch/SynchronizableChildPool.h"

SynchronizableChildPool *SynchronizableChildPool::make(SynchronizablePool *parent, uint16_t threadCount)
{
    if (parent == nullptr)
        throw runtime_error("parent can't be null");

    auto pointer = new SynchronizableChildPool(threadCount, parent);
    parent->attachedPools.push_back(unique_ptr<SynchronizableChildPool>(pointer));
    pointer->poolMutex = parent->poolMutex;

    return pointer;
}


SynchronizableChildPool::SynchronizableChildPool(uint16_t threadCount, SynchronizablePool *parent)
    : BasePool(threadCount, parent->poolMutex, parent->waitCondition, BasePool::internal())
{
    this->parent = parent;
    parent->runningThreads += this->threadCount;

    for ( uint16_t i = 0; i < this->threadCount; i++ ) {
        this->threads[i] = thread(
                [this] {
                    for (;;) {
                        function < void() > task;
                        {
                            unique_lock<mutex> lock(*this->poolMutex);

                            bool hadTasks = !this->tasks.empty() || !this->parent->tasks.empty();

                            // If there are currently no tasks to execute, the thread will enter the pause state and
                            // will stop running, so we decrement the number of running threads.
                            if (!hadTasks)
                            {
                                this->parent->runningThreads--;

                                // If there are no more running threads, wake all threads waiting for synchronization.
                                if (this->parent->runningThreads == 0)
                                    this->parent->synchronizeWait.notify_all();
                            }

                            this->waitCondition->wait(
                                    lock,
                                    [this] {
                                        return this->stop || !this->tasks.empty() || !this->parent->tasks.empty();
                                    }
                            );

                            // If there was no tasks to execute before the wait, it means the thread has woken, and is
                            // now running again -> increment number of running threads.
                            if (!hadTasks)
                                this->parent->runningThreads++;

                            if (this->stop && this->tasks.empty() && this->parent->tasks.empty())
                                return;

                            if (!this->tasks.empty())
                            {
                                task = move(this->tasks.front());
                                this->tasks.pop();
                            } else {
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
