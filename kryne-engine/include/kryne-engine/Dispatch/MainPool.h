/**
 * @file
 * @author Max Godefroy
 * @date 27/02/2021.
 */

#pragma once


#include <kryne-engine/Utils/Multithreading.hpp>
#include "SynchronizablePool.h"


class MainPool final : public BasePool {

public:

    MainPool(): id(this_thread::get_id()), mainMutex(&_mainMutex) {}

    template<class F, class... Args>
    future<result_of_t<F(Args...)>> enqueueFuture(F&& function, Args&& ...args)
    {
        using returnType = result_of_t<F(Args...)>;

        auto task = make_shared<packaged_task<returnType()>>(
                bind(forward<F>(function), forward<Args>(args)...)
        );

        future<returnType> result = task->get_future();
        {
            unique_lock<mutex> lock(*this->mainMutex);

            this->tasks.push([task] { (*task)(); });
        }

        waitCondition.notify_all();

        return result;
    }

    void enqueue(const function<void()>& func)
    {
        {
            unique_lock<mutex> lock(*this->mainMutex);
            this->tasks.push(func);
        }

        this->waitCondition.notify_all();
    }

    void swapQueues(queue<function<void()>> &swapQueue, bool allowNonEmpty) override;

    void synchronize(SynchronizablePool *pool);

    [[nodiscard]] inline bool isCurrentThread() const { return this_thread::get_id() == this->id; }

private:

    const thread::id id;

    mutex _mainMutex {};

    mutex *mainMutex;

    condition_variable_any waitCondition;

};


