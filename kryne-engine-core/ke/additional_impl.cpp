/**
 * @file
 * @author Max Godefroy
 * @date 10/10/2021.
 */


void* __cdecl operator new[](size_t size, const char* name, int flags, unsigned debugFlags, const char* file, int line)
{
    return new unsigned[size];
}

void* __cdecl operator new[](size_t size, size_t alignment, size_t alignment_offset, const char* name, int flags, unsigned debugFlags, const char* file, int line)
{
    return new unsigned[size];
}