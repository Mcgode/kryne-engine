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


using namespace std;


class Dispatcher {

public:

    Dispatcher();

    virtual ~Dispatcher();

private:

    /// Stop condition. Set it to true to stop all threads.
    bool stop = false;


public:

    void runBasicExecutionTask(function <void()> basicTask);

    void runBasicMainThreadTask(function <void()> basicTask);

protected:

    /// The count of base execution threads
    uint16_t threadCount;

    /// The ThreadObject objects representing the execution threads
    thread *executionThreads;

    /// The synchronization mutex for execution threads.
    mutex executionMutex;

    /// The execution wait condition.
    condition_variable executionCondition;

    /// Tasks that can be executed in parallel.
    queue<function<void()>> executionTasks;

    /// Tasks that must be run in the main thread.
    queue<function<void()>> mainThreadTasks;

};


