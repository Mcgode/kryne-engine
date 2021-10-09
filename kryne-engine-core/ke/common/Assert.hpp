/**
 * @file
 * @author Max Godefroy
 * @date 02/08/2021.
 */

#pragma once

#include <stdexcept>
#include <EASTL/string_view.h>


namespace KryneEngine
{
    void Assert(bool _condition, const eastl::string_view& _message)
    {
        if (!_condition)
            throw std::runtime_error(_message.begin());
    }

    void Assert(bool _condition)
    {
        Assert(_condition, "Condition not met");
    }
}