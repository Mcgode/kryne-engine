/**
 * @file
 * @author Max Godefroy
 * @date 27/02/2021.
 */

#include "kryne-engine/Dispatch/MainPool.h"

void MainPool::synchronize(SynchronizablePool *pool)
{
    pool->overrideSynchronizeWait(&this->waitCondition);
    for (;;)
    {
        function <void()> task;
        {
            unique_lock<mutex> lock(this->mainMutex);

            bool synchronize;
            this->waitCondition.wait(lock, [this, pool, &synchronize]
            {
                synchronize = pool->trySynchronize();
                return pool->trySynchronize() || !this->tasks.empty();
            });

            if (synchronize && this->tasks.empty())
                break;

            task = move(this->tasks.front());
            this->tasks.pop();
        }

        task();
    }
    pool->overrideSynchronizeWait(nullptr, true);
}
