#pragma once

namespace TS
{
    template <typename T, typename ... Params>
    T* MemorySystem::Alloc(Params ... params)
    {
        const size_t memorySize = sizeof(T) + sizeof(MemoryMetaData);

        auto pMemory = static_cast<char*>(_pAllocator->Alloc(memorySize));
        MemoryMetaData* block = new(pMemory)MemoryMetaData;
        pMemory += sizeof(MemoryMetaData);

        block->objectSize = sizeof(T);
        block->arrayCount = 1;
        block->typeData = typeid(T).name();

        //! 引数付コンストラクタを呼び出す
        T* pObject = new(pMemory)T(params ...);

        if (IsEnableMemoryLeak())
            RegisterMemoryMetaData(block);


        return pObject;
    }

    template <typename T>
    T* MemorySystem::AllocArray(size_t itemCount)
    {
        const size_t memorySize = sizeof(T) * itemCount + sizeof(MemoryMetaData);

        auto pMemory = static_cast<char*>(_pAllocator->Alloc(memorySize));
        MemoryMetaData* block = new(pMemory)MemoryMetaData;
        pMemory += sizeof(MemoryMetaData);
        
        block->objectSize = sizeof(T);
        block->arrayCount = itemCount;
        block->typeData = typeid(T).name();

        T* pCurrent = reinterpret_cast<T*>(pMemory);

        for (size_t i = 0; i < itemCount; ++i)
        {
            //! デフォルトコンストラクタを呼び出す
            new(pCurrent)T;
            ++pCurrent;
        }

        // ! メタ情報を埋め込んでおく
        if (IsEnableMemoryLeak())
            RegisterMemoryMetaData(block);

        return reinterpret_cast<T*>(pMemory);
    }

    template <typename T>
    void MemorySystem::Delete(T*& ptr)
    {
        IAllocator* pAllocator = GetAllocator();

        MemoryMetaData& meta = GetMemoryMetaDeta(ptr);

        void* pMemoryHead = &meta;
        
        T* pCurrent = ptr;

        // ! デストラクタを呼び出す
        for (unsigned i = 0; i < meta.arrayCount; ++i)
        {
            pCurrent[i].~T();
        }

        if (IsEnableMemoryLeak())
            RemoveMemoryMetaData(&meta);

        pAllocator->Free(pMemoryHead);
        ptr = nullptr;
    }

}