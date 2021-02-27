/**
 * @file
 * @author Max Godefroy
 * @date 25/02/2021.
 */

#pragma once


#include "ThreadPool.h"

class ChildPool;


class RunnerPool final : public ThreadPool {

    friend class ChildPool;

public:

    explicit RunnerPool(uint16_t threadCount);

    ~RunnerPool() override;

private:

    mutex _poolMutex {};

    condition_variable _waitCondition {};

    vector<unique_ptr<ChildPool>> attachedPools {};

};


#include "ChildPool.h"


