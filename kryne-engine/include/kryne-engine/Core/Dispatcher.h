/**
 * @file
 * @author Max Godefroy
 * @date 23/02/2021.
 */

#pragma once


#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <iostream>


using namespace std;


class Dispatcher {

public:

    Dispatcher();

    virtual ~Dispatcher();

private:

    /// Stop condition. Set it to true to stop all threads.
    bool stop = false;


public:

    void runBasicExecutionTask(function<void()> basicTask);

    void runBasicMainThreadTask(function<void()> basicTask);

    /**
     * Synchronizes the current thread to the execution threads, by waiting for them to stop running.
     * The current thread will sleep during this time.
     */
    void synchronize();

protected:

    inline bool hasExecutionTask(uint16_t i)
    {
        return !this->executionTasks.empty() || (i == 0 && !this->mainThreadTasks.empty());
    }

protected:

    /// The count of base execution threads
    uint16_t threadCount;

    /// The ThreadObject objects representing the execution threads
    thread *executionThreads;

    /// The amount of threads running at the same time
    uint16_t runningThreads = 0;

    /// The condition variable for waiting for the end of the tasks execution.
    condition_variable synchronizeCondition;

    /// The synchronization mutex for execution threads.
    mutex executionMutex;

    /// The execution wait condition.
    condition_variable executionCondition;

    /// Tasks that can be executed in parallel.
    queue<function<void()>> executionTasks;

    /// Tasks that must be run in the main thread.
    queue<function<void()>> mainThreadTasks;


public:

    void addAsyncTask(function<void()> asyncTask);

    void launchAsyncProcessing();

protected:

    queue<function<void()>> asyncTasks;

    mutex asyncMutex {};

};


