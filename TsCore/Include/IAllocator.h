#pragma once

#include "Common.h"

namespace TS
{
    class IAllocator
    {
    public:
        virtual ~IAllocator() = default;
        virtual void* Alloc(size_t size) = 0;
        virtual void Free(void * pointer) = 0;
    };

    /**
    * \brief OSから直接メモリの確保＆解放を行う場合はこのアロケータが利用できる
    */
    class OsAllocator : public IAllocator
    {
    public:
        void* Alloc(size_t size)override
        {
            return malloc(size);
        }

        void Free(void * pointer)override
        {
            free(pointer);
        }
    };
}
