/**
 * @file
 * @author max
 * @date 10/3/21.
 */

#include "Logger.hpp"

#include <common/meta/Enums.hpp>
#include <iostream>


namespace KryneEngine
{
    ENUM_META_BEGIN_TO_STRING(Logger::Level)
        ENUM_META(Logger::Level::Verbose, "VERBOSE")
        ENUM_META(Logger::Level::Info, "INFO")
        ENUM_META(Logger::Level::Warning, "WARNING")
        ENUM_META(Logger::Level::Error, "ERROR")
        ENUM_META(Logger::Level::Fatal, "FATAL")
    ENUM_META_END()

    ENUM_META_BEGIN_TO_STRING(Logger::Category)
        ENUM_META(Logger::Category::Core, "Core")
        ENUM_META(Logger::Category::Rendering, "Rendering")
    ENUM_META_END()

    void Logger::Log(const Logger::Level &_level, const Logger::Category &_cat, eastl::string_view _message)
    {
        std::cout << to_string(_level) << " - [" << to_string(_cat) << "] " << _message.begin() << std::endl;
    }
}