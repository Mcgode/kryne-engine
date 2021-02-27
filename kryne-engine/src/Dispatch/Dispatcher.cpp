/**
 * @file
 * @author Max Godefroy
 * @date 23/02/2021.
 */

#include "kryne-engine/Dispatch/Dispatcher.h"

Dispatcher::Dispatcher()
{
    uint16_t threadCount = thread::hardware_concurrency();

    if (threadCount == 0)
        threadCount = 4;

    this->parallelExecutionThreads = make_unique<SynchronizablePool>(threadCount - 1);
    this->mainThread = make_unique<MainPool>();

    this->ioPool = make_unique<RunnerPool>(2);
}
