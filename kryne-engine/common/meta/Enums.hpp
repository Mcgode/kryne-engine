/**
 * @file
 * @author Max Godefroy
 * @date 09/10/2021.
 */

#pragma once

#define ENUM_META_BEGIN(output_type, enum_type, meta_name) \
    constexpr output_type meta_name(enum_type _enumValue)  \
    {                                                      \
        switch (_enumValue) {

#define ENUM_META_BEGIN_TO_STRING(enum_type) ENUM_META_BEGIN(const char*, enum_type, to_string)


#define ENUM_META(enum, value) case enum: return value;


#define ENUM_META_END() }}