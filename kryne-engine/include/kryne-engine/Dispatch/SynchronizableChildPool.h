/**
 * @file
 * @author Max Godefroy
 * @date 25/02/2021.
 */

#pragma once


#include "ThreadPool.h"

class SynchronizablePool;


class SynchronizableChildPool final : public ThreadPool {

public:

    /**
     * @param parent
     * @param threadCount
     * @return
     */
    static SynchronizableChildPool *make(SynchronizablePool *parent, uint16_t threadCount);

private:

    explicit SynchronizableChildPool(uint16_t threadCount, SynchronizablePool *parent);

private:

    SynchronizablePool *parent;

};


#include "SynchronizablePool.h"


