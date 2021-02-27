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


/**
 * @brief A central multithreaded task dispatcher, in the form of a singleton class.
 *
 * @details
 * Different thread pools are attached to this dispatcher, and you can enqueue them with tasks to execute.
 */
class Dispatcher {


public:

    /**
     * Meyer's singleton instance of #Dispatcher
     */
    static Dispatcher &instance()
    {
        static Dispatcher dispatcher;
        return dispatcher;
    }

protected:

    /**
     * Initializes the multiple pools.
     */
    Dispatcher();



public:

    /**
     * @brief Retrieves the main thread pool.
     *
     * @details
     * This is a single-thread pool that will be used to synchronize the game loop (and, depending on the graphics API,
     * run API calls).
     *
     * This pool will synchronize with #parallel() twice every game loop: once to run the entire game loop, and once
     * more to run the delayed tasks. Synchronization means that the main thread will wait until all of its tasks
     * are finished executed and the same is true for the parallel pool.
     *
     * @warning
     * This queue should only be directly enqueued by the engine. If you need to enqueue main thread tasks from outside
     * the engine code, use #enqueueDelayed() instead.
     */
    [[nodiscard]] MainPool *main() const { return this->mainThread.get(); }

    /**
     * @brief Retrieves the parallel threads pool.
     *
     * @details
     * This pool will directly execute all the tasks that are enqueued to it, and can be synchronized with the current
     * thread (meaning, the current thread will be paused until all tasks are finished executing), though you should let
     * the engine handle that synchronization.
     *
     * It'll happen during every game loop, and will be synchronized with the main thread. This means that you should be
     * wary and only enqueue small tasks, or else you might make the main thread wait for too long. If you have bigger
     * tasks try breaking them into smaller tasks or run it in another pool or even a dedicated thread.
     *
     * @warning
     * This queue should only be directly enqueued by the engine. If you need to enqueue parallel tasks from outside the
     * engine code, use #enqueueDelayed() instead.
     */
    [[nodiscard]] SynchronizablePool *parallel() const { return this->parallelExecutionThreads.get(); }

    /**
     * @brief Waits for all the tasks in the main thread and the parallel threads to finish their execution before
     *        continuing.
     */
    inline void waitMain() const { mainThread->synchronize(parallelExecutionThreads.get()); }

protected:

    /// The main thread pool.
    unique_ptr<MainPool> mainThread;

    /// The parallel execution thread pool.
    unique_ptr<SynchronizablePool> parallelExecutionThreads;


public:

    /**
     * @brief Retrieves the IO thread pool.
     *
     * @details
     * Use this pool to execute IO tasks that will take some time. This thread pool is not synchronized with other pools
     * so time of execution is not too much of a concern.
     *
     * It is highly recommended to use #enqueueDelayed() to save the result of the task into game loop objects, to
     * limit data racing issues. Delayed enqueueing allows to resynchronize the data with the game loop.
     */
    [[nodiscard]] RunnerPool *io() const { return this->ioPool.get(); }

protected:

    /// The IO thread pool. Uses 2 threads.
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

