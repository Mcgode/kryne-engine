/**
 * @file
 * @author Max Godefroy
 * @date 27/02/2021.
 */

#pragma once


#include <queue>
#include <functional>


using namespace std;


class BasePool {

public:

    virtual void swapQueues(queue<function<void()>> &swapQueue, bool allowNonEmpty) = 0;

protected:

    queue<function<void()>> tasks {};

};


