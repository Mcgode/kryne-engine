/**
 * @file
 * @author Max Godefroy
 * @date 25/02/2021.
 */

#pragma once


#include "ThreadPool.h"


class SynchronizablePool final: public ThreadPool {

public:

    ~SynchronizablePool() override;

    explicit SynchronizablePool(uint16_t threadCount);

    void synchronize();

    [[nodiscard]] bool trySynchronize() const;

    void overrideSynchronizeWait(condition_variable_any *newCondition, bool notifyAll = false);

    [[nodiscard]] mutex *getMutex() const { return this->poolMutex; }

private:

    mutex _poolMutex {};

    condition_variable _waitCondition {};

    uint16_t runningThreads = 0;

    condition_variable_any *synchronizeWait;

    condition_variable_any _defaultWait {};

};


