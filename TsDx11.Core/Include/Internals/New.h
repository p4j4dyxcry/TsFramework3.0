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

    /**
    * \brief STL用アロケータ(試験実装)
    * \tparam T
    */
    template <class T>
    class stl_allocator : public std::allocator<T>
    {

    public:
        stl_allocator() { }

        stl_allocator(const stl_allocator& x){};

        template<class U>
        stl_allocator(const stl_allocator<U>& x){};

        T* allocate(size_t n)
        {            
            return GetGlobalMemorySystem().GetAllocator()->Alloc(n);
        }

        void deallocate(T* p, std::size_t n)
        {
            GetGlobalMemorySystem().GetAllocator()->Free(n);
        }

        template<class U>
        struct rebind { typedef stl_allocator<U> other; };
    };

    #define TS_MEMORYSYSTEM                 TS::GetGlobalMemorySystem().SetDebugInfo(TS::DebugInfo(__LINE__, __FILE__, __FUNCTION__))
    #define TS_NEW(__type__)                TS_MEMORYSYSTEM.Construct<__type__>
    #define TS_NEWARRAY(__type__ , count)   TS_MEMORYSYSTEM.Constructs<__type__>(count)
    #define TS_DELETE(pointer)              TS_MEMORYSYSTEM.Destruct(pointer)
}