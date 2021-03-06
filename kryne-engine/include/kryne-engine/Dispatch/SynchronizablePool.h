/**
 * @file
 * @author Max Godefroy
 * @date 25/02/2021.
 */

#pragma once


#include "ThreadPool.h"


class SynchronizablePool final: public ThreadPool {

public:

    explicit SynchronizablePool(uint16_t threadCount);

    void synchronize();

    bool trySynchronize() const;

    void overrideSynchronizeWait(condition_variable *newCondition, bool notifyAll = false);

    [[nodiscard]] mutex *getMutex() const { return this->poolMutex; }

private:

    mutex _poolMutex {};

    condition_variable _waitCondition {};

    uint16_t runningThreads = 0;

    condition_variable *synchronizeWait;

    condition_variable _defaultWait {};

};


