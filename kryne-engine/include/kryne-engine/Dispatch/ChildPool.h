/**
 * @file
 * @author Max Godefroy
 * @date 25/02/2021.
 */

#pragma once


#include "ThreadPool.h"

class RunnerPool;


class ChildPool final : public ThreadPool {

public:

    /**
     * @param parent
     * @param threadCount
     * @return
     */
    static ChildPool *make(RunnerPool *parent, uint16_t threadCount);

protected:

    explicit ChildPool(uint16_t threadCount, RunnerPool *parent);

protected:

    RunnerPool *parent;

};


#include "RunnerPool.h"

