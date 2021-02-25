/**
 * @file
 * @author Max Godefroy
 * @date 25/02/2021.
 */

#pragma once


#include "RunnerPool.h"


class ChildPool: public RunnerPool {

public:

    /**
     * @param parent
     * @param threadCount
     * @return
     */
    static ChildPool *make(RunnerPool *parent, uint16_t threadCount);

protected:

    explicit ChildPool(uint16_t threadCount);

    ChildPool(uint16_t threadCount, enum internal): RunnerPool(threadCount, internal()) {}

protected:

    RunnerPool *parent = nullptr;

};


