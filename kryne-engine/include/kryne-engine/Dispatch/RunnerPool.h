/**
 * @file
 * @author Max Godefroy
 * @date 25/02/2021.
 */

#pragma once


#include "ThreadPool.h"


class RunnerPool final : public ThreadPool {

public:

    explicit RunnerPool(uint16_t threadCount);

    virtual ~RunnerPool();

private:

    mutex _poolMutex {};

    condition_variable _waitCondition {};

};


