/**
 * @file
 * @author Max Godefroy
 * @date 25/02/2021.
 */

#pragma once


#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <future>
#include <memory>


using namespace std;


class BasePool {

public:

    virtual ~BasePool();

    template<class F, class... Args>
    future<result_of_t<F(Args...)>> enqueue(F&& function, Args&& ...args)
    {
        using returnType = result_of_t<F(Args...)>;

        auto task = make_shared<packaged_task<returnType()>>(
                bind(forward<F>(function), forward<Args>(args)...)
        );

        future<returnType> result = task->get_future();
        {
            unique_lock<mutex> lock(*this->poolMutex);

            if (this->stop)
                throw runtime_error("Tried to enqueue a stopped pool");

            tasks.emplace([task] { task.get()(); });
        }

        waitCondition->notify_one();

        return result;
    }


protected:

    enum internal {};

    BasePool(uint16_t threadCount, mutex *mutex, condition_variable *condition, enum internal);


protected:

    uint16_t threadCount;

    thread *threads;

    queue<function<void()>> tasks {};

    mutex *poolMutex;

    condition_variable *waitCondition;

    bool stop = false;

};


