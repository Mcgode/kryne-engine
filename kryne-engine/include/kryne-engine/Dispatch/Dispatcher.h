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
     * the engine code, use #enqueueMainDelayed() instead.
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
     * engine code, use #enqueueParallelDelayed() instead.
     */
    [[nodiscard]] SynchronizablePool *parallel() const { return this->parallelExecutionThreads.get(); }

    /**
     * @brief Waits for all the tasks in the main thread and the parallel threads to finish their execution before
     *        continuing.
     */
    inline void waitMain() const
    {
#if KRYNE_ENGINE_SINGLE_THREADED != 1
        mainThread->synchronize(parallelExecutionThreads.get());
#endif
    }

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
     * It is highly recommended to use #enqueueMainDelayed() or #enqueueParallelDelayed() to save the result of the task
     * into game loop objects, to limit data racing issues. Delayed enqueueing allows to resynchronize the data with the
     * game loop.
     */
    [[nodiscard]] RunnerPool *io() const { return this->ioPool.get(); }

protected:

    /// The IO thread pool. Uses 2 threads.
    unique_ptr<RunnerPool> ioPool;


public:

    /**
     * @brief Enqueues a task to be executed in the main thread
     *
     * @details
     * The task will be executed at the end of the game loop (hence the 'delayed' in the name) by the main thread.
     *
     * @see #main() for more documentation on the %main thread.
     *
     * @tparam F        Function type
     * @tparam Args     Function arguments packed typed
     * @param function  The task function to execute
     * @param args      The task function arguments
     * @return A future for the result of the enqueued task
     */
    template<class F, class... Args>
    inline future<result_of_t<F(Args...)>> enqueueMainDelayed(F&& function, Args&& ...args)
    {
        return enqueueDelayed(this->delayedMainQueue, function, args...);
    }

    /**
     * @brief Enqueues a task to be executed in the main thread
     *
     * @details
     * The task will be executed at the end of the game loop (hence the 'delayed' in the name) by the parallel threads.
     *
     * @see #parallel() for more documentation on the %parallel thread pool.
     *
     * @tparam F        Function type
     * @tparam Args     Function arguments packed typed
     * @param function  The task function to execute
     * @param args      The task function arguments
     * @return A future for the result of the enqueued task
     */
    template<class F, class... Args>
    inline future<result_of_t<F(Args...)>> enqueueParallelDelayed(F&& function, Args&& ...args)
    {
#if KRYNE_ENGINE_SINGLE_THREADED == 1
        return enqueueDelayed(this->delayedMainQueue, function, args...);
#else
        return enqueueDelayed(this->delayedParallelQueue, function, args...);
#endif
    }

    /**
     * @brief Executes and waits for all the delayed tasks.
     *
     * @details
     * Will swap queues for both the main thread and the parallel thread pool, with the delayed tasks queues of the
     * dispatcher (#delayedMainQueue and #delayedParallelQueue).
     *
     * Ensure that all tasks from both of them are executed (and thus flushed) first, since it is considered as a
     * precondition for the queue swapping. Do do so, call #waitMain() first. <br>
     * This is the main reason why you always want to enqueue into delayed queues. If you try to enqueue one of the
     * pools directly (using `%main()->enqueue()` for instance), you might run in a situation where #waitMain()
     * finished, another thread enqueues a new task directly, before #waitDelayed() is called and any queue swap can be
     * done, and thus breaks the precondition, leading to a crash.
     */
    inline void waitDelayed()
    {
        this->mainThread->swapQueues(this->delayedMainQueue, false);

#if KRYNE_ENGINE_SINGLE_THREADED != 1
        this->parallelExecutionThreads->swapQueues(this->delayedParallelQueue, false);
#endif

        this->waitMain();
    }

protected:

    /**
     * @brief Enqueues a task to the provided queue
     *
     * @tparam F        Function type
     * @tparam Args     Function arguments packed typed
     * @param taskQueue The queue to enqueue
     * @param function  The task function to execute
     * @param args      The task function arguments
     * @return A future for the result of the enqueued task
     */
    template<class F, class... Args>
    future<result_of_t<F(Args...)>> enqueueDelayed(queue<function<void()>> &taskQueue, F&& function, Args&& ...args)
    {
        using returnType = result_of_t<F(Args...)>;

        auto task = make_shared<packaged_task<returnType()>>(
                bind(forward<F>(function), forward<Args>(args)...)
        );

        future<returnType> result = task->get_future();
        {
            scoped_lock<mutex> lock(this->delayedMutex);

            taskQueue.emplace([task] { (*task)(); });
        }

        return result;
    }

protected:

    /// The delayed task queue for the main thread.
    queue<function<void()>> delayedMainQueue {};

    /// The delayed task queue for the parallel thread pool.
    queue<function<void()>> delayedParallelQueue {};

    /// The mutex for handling concurrent write of delayed queues
    mutex delayedMutex {};

};


void inline assertIsMainThread()
{
#ifndef KRYNE_ENGINE_MAIN_THREAD_ASSERT_DISABLE

    if (!Dispatcher::instance().main()->isCurrentThread())
    {
        throw std::runtime_error("This is not the main thread");
    }

#endif
}

