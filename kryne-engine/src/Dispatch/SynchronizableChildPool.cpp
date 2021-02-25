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

    if (dynamic_cast<SynchronizableChildPool *>(parent) != nullptr)
        throw runtime_error("parent can't be another SynchronizableChildPool.");

    auto pointer = new SynchronizableChildPool(threadCount, parent);
    parent->attachedPools.push_back(unique_ptr<RunnerPool>(pointer));
    pointer->poolMutex = parent->poolMutex;

    return pointer;
}


SynchronizableChildPool::SynchronizableChildPool(uint16_t threadCount, SynchronizablePool *parent)
    : SynchronizableChildPool(threadCount, parent, RunnerPool::internal())
{
    this->runningThreads = this->threadCount;
    parent->runningThreads += this->threadCount;

    for ( uint16_t i = 0; i < this->threadCount; i++ ) {
        this->threads[i] = thread(
                [this] {
                    for (;;) {
                        function < void() > task;
                        {
                            unique_lock<mutex> lock(*this->poolMutex);

                            bool hasTask = !this->tasks.empty() || !this->parent->tasks.empty();

                            // If there are currently no tasks to execute, the thread will enter the pause state and
                            // will stop running, so we decrement the number of running threads.
                            if (!hasTask)
                            {
                                this->runningThreads--;
                                this->parent->runningThreads--;

                                // If there are no more running threads, wake all threads waiting for synchronization.
                                if (this->runningThreads == 0)
                                    this->synchronizeWait.notify_all();
                                if (this->parent->runningThreads == 0)
                                    this->parent->synchronizeWait.notify_all();
                            }

                            this->waitCondition.wait(
                                    lock,
                                    [this, hasTask] {
                                        return this->stop || hasTask;
                                    }
                            );

                            // If there was no tasks to execute before the wait, it means the thread has woken, and is
                            // now running again -> increment number of running threads.
                            if (!hasTask)
                            {
                                this->runningThreads++;
                                this->parent->runningThreads++;
                            }

                            if (this->stop && this->tasks.empty())
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
