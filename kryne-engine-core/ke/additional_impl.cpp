/**
 * @file
 * @author Max Godefroy
 * @date 10/10/2021.
 */

#include <stdlib.h>


#if defined(_MSC_VER)

    void* __cdecl operator new[](size_t size, const char* name, int flags, unsigned debugFlags, const char* file, int line)
    {
        return new unsigned[size];
    }

    void* __cdecl operator new[](size_t size, size_t alignment, size_t alignment_offset, const char* name, int flags, unsigned debugFlags, const char* file, int line)
    {
        return new unsigned[size];
    }

#elif defined(__GNUC__)

    void* operator new[](size_t size, const char* pName, int flags, unsigned debugFlags, const char* file, int line)
    {
        return malloc(size);
    }

    void* operator new[](size_t size, size_t alignment, size_t alignmentOffset, const char* pName, int flags, unsigned debugFlags, const char* file, int line)
    {
        return malloc(size);
    }

#else
#error No definition for your compiler
#endif