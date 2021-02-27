/**
 * @file
 * @author Max Godefroy
 * @date 23/02/2021.
 */

#pragma once


#include "SynchronizablePool.h"
#include "RunnerPool.h"
#include "MainPool.h"


using namespace std;


class Dispatcher {


public:

    static Dispatcher &instance()
    {
        static Dispatcher dispatcher;
        return dispatcher;
    }

protected:

    Dispatcher();



public:

    [[nodiscard]] MainPool *main() const { return this->mainThread.get(); }

    [[nodiscard]] SynchronizablePool *parallel() const { return this->parallelExecutionThreads.get(); }

    inline void waitMain() const { mainThread->synchronize(parallelExecutionThreads.get()); }

protected:

    unique_ptr<SynchronizablePool> parallelExecutionThreads;

    unique_ptr<MainPool> mainThread;


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

            delayedQueue.emplace([task] { (*task)(); });
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

#ifdef KRYNE_ENGINE_MAIN_THREAD_ASSERT_DISABLE

    #define assertIsMainThread() ""

#else

    #define assertIsMainThread() assert(("This is not the main thread", Dispatcher::instance().main()->isCurrentThread()))

#endif

