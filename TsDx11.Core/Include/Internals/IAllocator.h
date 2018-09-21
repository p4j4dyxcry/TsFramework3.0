#pragma once

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
    * \brief OS���璼�ڃ������̊m�ہ�������s���ꍇ�͂��̃A���P�[�^�����p�ł���
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
