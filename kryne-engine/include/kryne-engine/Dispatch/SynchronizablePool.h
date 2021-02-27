/**
 * @file
 * @author Max Godefroy
 * @date 25/02/2021.
 */

#pragma once


#include "BasePool.h"

class SynchronizableChildPool;


class SynchronizablePool final: public BasePool {

    friend class SynchronizableChildPool;

public:

    explicit SynchronizablePool(uint16_t threadCount);

    void synchronize();

    bool trySynchronize();

    void overrideSynchronizeWait(condition_variable *newCondition, bool notifyAll = false);

    ~SynchronizablePool() override;

private:

    mutex _poolMutex {};

    condition_variable _waitCondition {};

    vector<unique_ptr<SynchronizableChildPool>> attachedPools {};

    uint16_t runningThreads = 0;

    condition_variable *synchronizeWait;

    condition_variable _defaultWait {};

};


#include "SynchronizableChildPool.h"


