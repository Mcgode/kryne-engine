/**
 * @file
 * @author max
 * @date 7/30/21.
 */

#pragma once

#include <EASTL/shared_ptr.h>

class GraphicsApi
{
public:

    static ThreadInit(eastl::shared_ptr<GraphicsApi>& _api)
    {
        s_currentApi = _api;
    }

private:

    static thread_local eastl::shared_ptr<GraphicsApi> s_currentApi;
};


