/**
 * @file
 * @author Max Godefroy
 * @date 23/02/2021.
 */

#pragma once


#include "SynchronizablePool.h"
#include "RunnerPool.h"


using namespace std;


class Dispatcher {

public:

    Dispatcher();


public:

    [[nodiscard]] SynchronizableChildPool *main() const { return mainThread; }

    [[nodiscard]] SynchronizablePool *parallel() const { return this->parallelExecutionThreads.get(); }

protected:

    unique_ptr<SynchronizablePool> parallelExecutionThreads;

    SynchronizableChildPool *mainThread;


public:

    [[nodiscard]] RunnerPool *io() const { return this->ioPool.get(); }

protected:

    unique_ptr<RunnerPool> ioPool;


public:

    template<class F, class... Args>
    future<result_of_t<F(Args...)>> enqueueDelayed(F&& function, Args&& ...args)
    {
        using returnType = result_of_t<F(Args...)>;

        auto task = make_shared<packaged_task<returnType()>>(
                bind(forward<F>(function), forward<Args>(args)...)
        );

        future<returnType> result = task->get_future();
        {
            unique_lock<mutex> lock(this->delayedMutex);

            delayedQueue.emplace([task] { task.get()(); });
        }

        return result;
    }

    inline void synchronizeDelayed()
    {
        this->delayedPool->swapQueues(this->delayedQueue);
        this->delayedPool->synchronize();
    }

protected:

    unique_ptr<SynchronizablePool> delayedPool;

    queue<function<void()>> delayedQueue;

    mutex delayedMutex {};

};


