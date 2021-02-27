/**
 * @file
 * @author Max Godefroy
 * @date 27/02/2021.
 */

#pragma once


#include "SynchronizablePool.h"


class MainPool final {

public:

    MainPool(): id(this_thread::get_id()) {}

    template<class F, class... Args>
    future<result_of_t<F(Args...)>> enqueue(F&& function, Args&& ...args)
    {
        using returnType = result_of_t<F(Args...)>;

        auto task = make_shared<packaged_task<returnType()>>(
                bind(forward<F>(function), forward<Args>(args)...)
        );

        future<returnType> result = task->get_future();
        {
            unique_lock<mutex> lock(this->mainMutex);

            tasks.emplace([task] { (*task)(); });
        }

        waitCondition.notify_all();

        return result;
    }

    void swapQueues(queue<function<void()>> &swapQueue, bool allowNonEmpty = false);

    void synchronize(SynchronizablePool *pool);

    [[nodiscard]] inline bool isCurrentThread() const { return this_thread::get_id() == this->id; }

private:

    const thread::id id;

    mutex mainMutex {};

    condition_variable waitCondition;

    queue<function<void()>> tasks {};

};


