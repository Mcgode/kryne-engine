/**
 * @file
 * @author Max Godefroy
 * @date 23/02/2021.
 */

#include "Dispatcher.h"

Dispatcher::Dispatcher()
{
    this->threadCount = thread::hardware_concurrency();

    // If hardware thread count couldn't be detected, default to 4 execution threads.
    if (this->threadCount == 0)
        this->threadCount = 4;

    this->executionThreads = new thread[this->threadCount];

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
                            this->executionCondition.wait(
                                    lock,
                                    [this, i]
                                    {
                                        return this->stop ||
                                               !this->executionTasks.empty() ||
                                               (i == 0 && !this->mainThreadTasks.empty());
                                    }
                            );

                            if (this->stop && this->executionTasks.empty() && (i != 0 || this->mainThreadTasks.empty()))
                                return;

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
