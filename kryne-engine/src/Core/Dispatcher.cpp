/**
 * @file
 * @author Max Godefroy
 * @date 23/02/2021.
 */

#include "kryne-engine/Core/Dispatcher.h"

Dispatcher::Dispatcher()
{
    this->threadCount = thread::hardware_concurrency();

    // If hardware thread count couldn't be detected, default to 4 execution threads.
    if (this->threadCount == 0)
        this->threadCount = 4;

    this->executionThreads = new thread[this->threadCount];

    // By default, all threads are considered to be running.
    this->runningThreads = this->threadCount;

    for ( uint16_t i = 0; i < this->threadCount; i++ )
    {
        this->executionThreads[i] = thread(
                [this, i]
                {
                    for (;;)
                    {
                        function<void()> task;
                        {
                            unique_lock<mutex> lock(this->executionMutex);

                            bool hasExecutionTask = this->hasExecutionTask(i);

                            // If there are currently no tasks to execute, the thread will enter the pause state and
                            // will stop running, so we decrement the number of running threads.
                            if (!hasExecutionTask)
                                this->runningThreads--;

                            this->executionCondition.wait(
                                    lock,
                                    [this, hasExecutionTask]
                                    {
                                        return this->stop || hasExecutionTask;
                                    }
                            );

                            // If there was no tasks to execute before the wait, it means the thread has woken, and is
                            // now running again -> increment number of running threads.
                            if (!hasExecutionTask)
                                this->runningThreads++;

                            if (this->stop && !hasExecutionTask)
                                return;

                            // TODO: Separate main thread function code from regular thread code, for optimization.
                            // If index is 0, it's the main thread, and as such it needs to run main threads task in
                            // priority
                            if (i == 0 && !this->mainThreadTasks.empty())
                            {
                                task = move(this->mainThreadTasks.front());
                                this->mainThreadTasks.pop();
                            }
                            else
                            {
                                task = this->executionTasks.front();
                                this->executionTasks.pop();
                            }

                        }
                        task();
                    }
                }
        );
    }
}


Dispatcher::~Dispatcher()
{
    {
        unique_lock<mutex> lock(this->executionMutex);
        this->stop = true;
    }
    this->executionCondition.notify_all();

    for ( uint16_t i = 0; i < this->threadCount; i++ )
        this->executionThreads[i].join();

    delete this->executionThreads;
}


void Dispatcher::synchronize()
{
    unique_lock<mutex> lock(this->executionMutex);
    this->synchronizeCondition.wait(lock, [this] { return this->runningThreads == 0; });
}


void Dispatcher::runBasicExecutionTask(function<void()> basicTask)
{
    {
        unique_lock<mutex> lock(this->executionMutex);
        this->executionTasks.push(move(basicTask));
    }
    this->executionCondition.notify_one();
}


void Dispatcher::runBasicMainThreadTask(function<void()> basicTask)
{
    {
        unique_lock<mutex> lock(this->executionMutex);
        this->mainThreadTasks.push(move(basicTask));
    }
    this->executionCondition.notify_one();
}


void Dispatcher::addAsyncTask(function<void()> asyncTask)
{
    unique_lock<mutex> lock( this->asyncMutex );
    this->asyncTasks.push(move(asyncTask));
}


void Dispatcher::launchAsyncProcessing()
{
    unique_lock<mutex> lock1(this->asyncMutex);
    unique_lock<mutex> lock2(this->executionMutex);

    if (!this->executionTasks.empty() && !this->mainThreadTasks.empty())
    {
        cerr << "Task queue is not empty yet." << endl;
        return;
    }

    swap(this->executionTasks, this->asyncTasks);
}
