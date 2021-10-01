/**
 * @file
 * @author max
 * @date 7/30/21.
 */

#pragma once

#include <EASTL/shared_ptr.h>
#include <common/Assert.hpp>
#include <graphics/core/Texture.hpp>

namespace KryneEngine
{
    class Window;

    class GraphicsApiContext
    {
    public:
        /**
         * @brief A function to call at the start of a thread
         *
         * @details
         * This function will register the graphics API of the current thread in a thread-local global variable.
         * This will allow easy access to the API without limiting ourselves to one single global API.
         * The thread will own the API, to avoid invalid pointer issues. As suc, it is expected from the developer
         * to kill all the owning threads or to release it manually from every owning thread.
         *
         * @param _api A shared pointer of the graphics API context this thread will run on.
         */
        static void ThreadInit(eastl::shared_ptr<GraphicsApiContext>& _api)
        {
            s_currentApi = _api;
        }

        /**
         * @brief Returns true if the thread owns an API context
         */
        static bool HasAssociatedContext()
        {
            return s_currentApi != nullptr;
        }

        /**
         * @brief Retrieves this thread's associated graphics API
         *
         * @return A reference to the graphics API for this thread
         */
        static GraphicsApiContext& Get()
        {
            Assert(s_currentApi != nullptr, "No context available. Have you set it up properly?");
            return *s_currentApi;
        }


        // =========================================
        //  Associated window
        // =========================================

        /// @brief Retrieves the current main window for this graphic context
        [[nodiscard]] Window* GetAssociatedWindow() const
        {
            return m_associatedWindow;
        }

    protected:
        explicit GraphicsApiContext(Window* _contextWindow): m_associatedWindow(_contextWindow) {}

        Window* m_associatedWindow;


        // =========================================
        //  Textures
        // =========================================

    public:
        virtual eastl::shared_ptr<Texture>&& CreateTexture(const Texture::Description& _desc) = 0;

    private:
        /// The graphics API used on this thread. Initialized by #ThreadInit()
        static thread_local eastl::shared_ptr<GraphicsApiContext> s_currentApi;
    };
}


