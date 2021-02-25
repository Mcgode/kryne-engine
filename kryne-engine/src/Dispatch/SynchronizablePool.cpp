/**
 * @file
 * @author Max Godefroy
 * @date 25/02/2021.
 */

#include <ios>
#include "kryne-engine/Dispatch/SynchronizablePool.h"


SynchronizablePool::SynchronizablePool(uint16_t threadCount, RunnerPool::internal internal)
        : RunnerPool(threadCount, internal)
{}


SynchronizablePool::SynchronizablePool(uint16_t threadCount) : SynchronizablePool(threadCount, RunnerPool::internal())
{
    // By default, all threads are running.
    this->runningThreads = this->threadCount;

    for ( uint16_t i = 0; i < this->threadCount; i++ ) {
        this->threads[i] = thread(
                [this] {
                    for (;;) {
                        function < void() > task;
                        {
                            unique_lock<mutex> lock(*this->poolMutex);

                            bool hasTask = !this->tasks.empty();

                            // If there are currently no tasks to execute, the thread will enter the pause state and
                            // will stop running, so we decrement the number of running threads.
                            if (!hasTask)
                            {
                                this->runningThreads--;

                                // If there are no more running threads, wake all threads waiting for synchronization.
                                if (this->runningThreads == 0)
                                    this->synchronizeWait.notify_all();
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
                                this->runningThreads++;

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


void SynchronizablePool::synchronize()
{
    unique_lock<mutex> lock(*this->poolMutex);
    this->synchronizeWait.wait(lock, [this] { return this->runningThreads == 0; });
}
