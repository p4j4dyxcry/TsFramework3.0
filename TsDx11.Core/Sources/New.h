#pragma once

namespace TS
{
    inline MemorySystem& GetGlobalMemorySystem()
    {
        //static OsAllocator _allocator;
        static Allocator _allocator(1024 * 1024 * 256);
        static MemorySystem global(&_allocator, true);
        return global;
    }

    #define TS_MEMORYSYSTEM                 TS::GetGlobalMemorySystem().SetDebugInfo(DebugInfo(__LINE__, __FILE__, __FUNCTION__))
    #define TS_NEW(__type__)                TS_MEMORYSYSTEM.Alloc<__type__>
    #define TS_NEWARRAY(__type__ , count)   TS_MEMORYSYSTEM.AllocArray<__type__>(count)
    #define TS_DELETE(pointer)              TS_MEMORYSYSTEM.Delete(pointer)
}