/**
 * @file
 * @author max
 * @date 7/30/21.
 */

#pragma once

#include <EASTL/shared_ptr.h>
#include "Window.hpp"

namespace KryneEngine
{
    class GraphicsApiContext
    {
    public:
        /**
         * @brief A function to call at the start of a thread
         *
         * @details
         * This function will register the graphics API of the current thread in a thread-local global variable.
         * This will allow easy access to the API without limiting ourselves to one single global API.
         *
         * @param _api A shared pointer of the graphics API context this thread will run on.
         */
        static void ThreadInit(eastl::shared_ptr<GraphicsApiContext>& _api)
        {
            s_currentApi = _api;
        }

        /// @brief Retrieves the current main window for this graphic context
        [[nodiscard]] virtual const Window& GetWindow() const = 0;

    private:
        /// The graphics API used on this thread. Initialized by #ThreadInit()
        static thread_local eastl::shared_ptr<GraphicsApiContext> s_currentApi;
    };
}


