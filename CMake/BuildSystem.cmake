
# Parse build type
string(REPLACE " " ";" BUILD_TYPE ${CMAKE_BUILD_TYPE})

list(LENGTH BUILD_TYPE TOTAL_BUILD_ARGS_LENGTH)

if (${TOTAL_BUILD_ARGS_LENGTH} GREATER_EQUAL 1)
    list(GET BUILD_TYPE 0 Arg)

    if (Arg STREQUAL "DebugOptCore")
        set(BuildLevel 1)
    elseif (Arg STREQUAL "DebugOptEngine")
        set(BuildLevel 2)
    elseif (Arg STREQUAL "Release")
        set(BuildLevel 3)
    elseif (Arg STREQUAL "Benchmark")
        set(BuildLevel 4)
    elseif (Arg STREQUAL "Final")
        set(BuildLevel 5)
    endif()
endif()

if (NOT DEFINED BuildLevel)
    set(BuildLevel 0)
endif()

message(STATUS "Build level: " ${BuildLevel})

if (${TOTAL_BUILD_ARGS_LENGTH} GREATER_EQUAL 2)
    list(GET BUILD_TYPE 1 Arg)

    if (Arg STREQUAL "VK")
        set(GraphicsApi "VK")
    endif()
endif()

if (NOT DEFINED GraphicsApi)
    set(GraphicsApi "GL")
endif()

message(STATUS "Using API: " ${GraphicsApi})
add_definitions("-DGRAPHICS_API_${GraphicsApi}")


# Setting up debug flags

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    # using Clang
    message(FATAL_ERROR "Clang flags not set up")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # using GCC
    set(CommonCompileFlags "")

    set(DebugCompileFlags -Og)
    set(DebugLinkFlags "")

    set(ReleaseCompileFlags -O1 -finline-small-functions)
    set(ReleaseLinkFlags "")

    set(FinalCompileFlags -O3)
    set(FinalLinkFlags "")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # using Visual Studio C++
    set(CommonCompileFlags /MDd /D_ITERATOR_DEBUG_LEVEL=0)

    set(DebugCompileFlags /Od /Ob1 /D_DEBUG /Zi ${CommonCompileFlags})
    set(DebugLinkFlags /debug /INCREMENTAL)

    set(ReleaseCompileFlags /O2 /Ob2 /D_DEBUG /Zi ${CommonCompileFlags})
    set(ReleaseLinkFlags /debug /INCREMENTAL)

    set(FinalCompileFlags /O2 /Ob3 /D NDEBUG ${CommonCompileFlags})
    set(FinalLinkFlags /INCREMENTAL:NO)
else()
    message(ERROR "Unknown compiler ${CMAKE_CXX_COMPILER_ID}")
endif()


# Set target flags function

function(SetTargetFlags Target)
    if (BuildLevel GREATER_EQUAL 4) # Benchmark or Final build
        message(STATUS "[BuildSystem] '${Target}' will use 'Final' flags")
        target_compile_options(${Target} PRIVATE ${FinalCompileFlags})
        target_link_options(${Target} PRIVATE ${FinalLinkFlags})

    else()
        get_target_property(Type ${Target} Type)

        if ((Type STREQUAL "Core") AND (BuildLevel GREATER_EQUAL 1))
            message(STATUS "[BuildSystem] '${Target}' will use 'Release' flags")
            target_compile_options(${Target} PRIVATE ${ReleaseCompileFlags})
            target_link_options(${Target} PRIVATE ${ReleaseLinkFlags})

        elseif ((Type STREQUAL "Engine") AND (BuildLevel GREATER_EQUAL 2))
            message(STATUS "[BuildSystem] '${Target}' will use 'Release' flags")
            target_compile_options(${Target} PRIVATE ${ReleaseCompileFlags})
            target_link_options(${Target} PRIVATE ${ReleaseLinkFlags})

        elseif (BuildLevel GREATER_EQUAL 3)
            message(STATUS "[BuildSystem] '${Target}' will use 'Release' flags")
            target_compile_options(${Target} PRIVATE ${ReleaseCompileFlags})
            target_link_options(${Target} PRIVATE ${ReleaseLinkFlags})

        elseif ((Type STREQUAL "External") AND (BuildLevel GREATER 0))
            message(STATUS "[BuildSystem] '${Target}' will use 'Release' flags")
            target_compile_options(${Target} PRIVATE ${ReleaseCompileFlags})
            target_link_options(${Target} PRIVATE ${ReleaseLinkFlags})

        else ()
            message(STATUS "[BuildSystem] '${Target}' will use 'Debug' flags")
            target_compile_options(${Target} PRIVATE ${DebugCompileFlags})
            target_link_options(${Target} PRIVATE ${DebugLinkFlags})

        endif()
    endif()
endfunction()

set(CMAKE_BUILD_TYPE "")