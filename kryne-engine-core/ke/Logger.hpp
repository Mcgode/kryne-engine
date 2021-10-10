/**
 * @file
 * @author max
 * @date 10/3/21.
 */

#pragma once

#include <EASTL/string_view.h>
#include <EASTL/string.h>

#if defined(_DEBUG)
    #define ENABLE_LOGGING
#endif

namespace KryneEngine
{
    class Logger {
    public:
        enum class Level {
            Verbose,
            Info,
            Warning,
            Error,
            Fatal
        };

        static void Log(Level _level, const eastl::string_view &_cat, const eastl::string_view &_message);

        template<class... Args>
        static inline void
        Log(Level _level, const eastl::string_view &_cat, const eastl::string_view &_message, Args &&... _args)
        {
#if defined(ENABLE_LOGGING)
            auto string = eastl::string().sprintf(_message.begin(), eastl::forward<Args>(_args)...);
            Log(_level, _cat, string);
#endif
        }

        enum class CommonCategories {
            Core,
            Rendering,
        };

        static void Log(Level _level, CommonCategories _cat, const eastl::string_view &_message);

        template<class... Args>
        static inline void
        Log(Level _level, CommonCategories _cat, const eastl::string_view &_message, Args &&... _args)
        {
#if defined(ENABLE_LOGGING)
            auto string = eastl::string().sprintf(_message.begin(), eastl::forward<Args>(_args)...);
            Log(_level, _cat, string);
#endif
        }

        #define LOG(level, cat, message, ...) Logger::Log(Logger::Level::level, cat, message, ##__VA_ARGS__)
        #define LOG_VERBOSE(cat, message, ...) LOG(Verbose, cat, message, ##__VA_ARGS__)
        #define LOG_INFO(cat, message, ...) LOG(Info, cat, message, ##__VA_ARGS__)
        #define LOG_WARNING(cat, message, ...) LOG(Warning, cat, message, ##__VA_ARGS__)
        #define LOG_ERROR(cat, message, ...) LOG(Error, cat, message, ##__VA_ARGS__)
        #define LOG_FATAL(cat, ...) LOG(Fatal, cat, message, ##__VA_ARGS__)

        #define LOG_IF(condition, level, cat, message, ...) if (condition) LOG(level, cat, message, ##__VA_ARGS__)
        #define LOG_IF_VERBOSE(condition, cat, message, ...) LOG_IF(condition, Verbose, cat, message, ##__VA_ARGS__)
        #define LOG_IF_INFO(condition, cat, message, ...) LOG_IF(condition, Info, cat, message, ##__VA_ARGS__)
        #define LOG_IF_WARNING(condition, cat, message, ...) LOG_IF(condition, Warning, cat, message, ##__VA_ARGS__)
        #define LOG_IF_ERROR(condition, cat, message, ...) LOG_IF(condition, Error, cat, message, ##__VA_ARGS__)
        #define LOG_IF_FATAL(condition, cat, message, ...) LOG_IF(condition, Fatal, cat, message, ##__VA_ARGS__)
    };
}


