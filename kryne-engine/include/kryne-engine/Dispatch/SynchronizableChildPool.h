/**
 * @file
 * @author Max Godefroy
 * @date 25/02/2021.
 */

#pragma once


#include "SynchronizablePool.h"

class SynchronizableChildPool: public SynchronizablePool {

public:

    /**
     * @param parent
     * @param threadCount
     * @return
     */
    static SynchronizableChildPool *make(SynchronizablePool *parent, uint16_t threadCount);

protected:

    explicit SynchronizableChildPool(uint16_t threadCount, SynchronizablePool *parent);

    SynchronizableChildPool(uint16_t threadCount, SynchronizablePool *parent, enum internal): SynchronizablePool(threadCount, internal()), parent(parent) {}

protected:

    SynchronizablePool *parent;

};


