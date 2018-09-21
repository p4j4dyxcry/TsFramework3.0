#pragma once

namespace TS
{
#define TS_USE_OS_ALLOCATOR 0
    inline MemorySystem& GetGlobalMemorySystem()
    {
#if TS_USE_OS_ALLOCATOR
        static OsAllocator _allocator;
# else
        static Allocator _allocator(1024 * 1024 * 256);
#endif
        static MemorySystem global(&_allocator, true);
        return global;
    }

    #define TS_MEMORYSYSTEM                 TS::GetGlobalMemorySystem().SetDebugInfo(DebugInfo(__LINE__, __FILE__, __FUNCTION__))
    #define TS_NEW(__type__)                TS_MEMORYSYSTEM.Construct<__type__>
    #define TS_NEWARRAY(__type__ , count)   TS_MEMORYSYSTEM.Constructs<__type__>(count)
    #define TS_DELETE(pointer)              TS_MEMORYSYSTEM.Destruct(pointer)
}