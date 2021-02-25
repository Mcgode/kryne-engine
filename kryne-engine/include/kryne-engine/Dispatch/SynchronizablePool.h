/**
 * @file
 * @author Max Godefroy
 * @date 25/02/2021.
 */

#pragma once


#include "RunnerPool.h"


class SynchronizablePool: public RunnerPool {

    friend class SynchronizableChildPool;

public:

    explicit SynchronizablePool(uint16_t threadCount);

    void synchronize();

protected:

    SynchronizablePool(uint16_t threadCount, internal internal);

protected:

    uint16_t runningThreads = 0;

    condition_variable synchronizeWait {};

};


