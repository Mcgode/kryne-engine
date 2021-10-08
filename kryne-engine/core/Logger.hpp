/**
 * @file
 * @author max
 * @date 10/3/21.
 */

#pragma once

#include <EASTL/string_view.h>
#include <EASTL/string.h>

namespace KryneEngine
{
    class Logger
    {
    public:
        enum class Level
        {
            Verbose,
            Info,
            Warning,
            Error,
            Fatal
        };

        enum class Category
        {
            Core,
            Rendering,
        };

        static void Log(const Level& _level, const Category& _cat, eastl::string_view _message);

        template<class... Args>
        static inline void Log(const Level& _level, const Category& _cat, eastl::string_view _message, Args&&... _args)
        {
            auto string = eastl::string().sprintf(_message.begin(), eastl::forward<Args>(_args)...);
            Log(_level, _cat, string);
        }

        
    };
}


