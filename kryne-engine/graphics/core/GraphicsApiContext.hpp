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
        // =========================================
        //  Init / delete
        // =========================================

    protected:
        /**
         * @brief Initializes the context and associate a window to it.
         */
        explicit GraphicsApiContext(Window& _contextWindow): m_associatedWindow(_contextWindow) {}

    public:
        /// Default destructor, for inheritance.
        virtual ~GraphicsApiContext() = default;


        // =========================================
        //  Get / set context
        // =========================================

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
        static void ThreadSetContext(eastl::shared_ptr<GraphicsApiContext>& _api)
        {
            s_currentApi = _api;
        }

        /// @brief Returns true if the thread owns an API context
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

        /**
         * @brief Marks this thread as the main thread for this context.
         *
         * @details
         * Some APIs, like OpenGL, need the current thread to "own" the context to run API commands.
         */
        virtual void SetMainThread() = 0;


        // =========================================
        //  Associated window
        // =========================================

    public:
        /// @brief Retrieves the current main window for this graphic context
        [[nodiscard]] Window& GetAssociatedWindow() const
        {
            return m_associatedWindow;
        }

    protected:
        /// The window this context is spawned from
        Window& m_associatedWindow;


        // =========================================
        //  Textures
        // =========================================

    public:
        /**
         * @brief Creates a blank, unmapped, texture in the graphics context.
         *
         * @warning
         * You still need to acquire memory for this texture before using it, using Texture::AcquireMemory()
         *
         * @param _desc The description of the texture.
         */
        virtual eastl::shared_ptr<Texture>&& CreateTexture(const Texture::Description& _description) = 0;

    private:
        /// The graphics API used on this thread. Initialized by #ThreadInit()
        static thread_local eastl::shared_ptr<GraphicsApiContext> s_currentApi;
    };
}


