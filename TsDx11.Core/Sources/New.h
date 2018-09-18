#pragma once

namespace TS
{

#define TS_NEW(__type__)        TS::MetaDataIncludeConstructor<__type__>(__LINE__,__FILE__,__FUNCTION__)
#define TS_NEWARRAY(type , count)   TS::AllocArray<type>(__LINE__,__FILE__,__FUNCTION__,count)
#define TS_DELETE(pointer) TS::DeleteMemory(pointer)

    inline MemorySystem& GetGlobalMemorySystem()
    {
        static DefaultAllocator _allocator;
        static MemorySystem global(&_allocator,true);
        return global;
    }


    /**
    * \brief メモリのメタデータを取得します。
    * \param pointer メモリを開放するポインタ
    */
    inline MemoryMetaData& GetMemoryMetaDeta(void* pointer)
    {
        return *reinterpret_cast<MemoryMetaData*>(static_cast<char*>(pointer) - sizeof(MemoryMetaData));
    }

    /**
    * \brief メタ情報を埋め込みつつ可変長コンストラクタを呼ぶ仕組み
    */
    template<typename TypeX>
    struct MetaDataIncludeConstructor
    {
        struct FuncData
        {
            int line;
            const char* filename;
            const char* function;
        };

        //メタ情報の埋め込み
        FuncData metaData;
        MetaDataIncludeConstructor(const int line,
            const char* filename,
            const char* functionname)
        {
            metaData.line = line;
            metaData.filename = filename;
            metaData.function = functionname;
        }

        //! 対象のメモリを確保しコンストラクタを呼び出す
        /**
        * \brief対象のメモリを確保しコンストラクタを呼び出す
        * \param params 作成するクラスのコンストラクタ引数
        * \return 作成されたオブジェクト
        */
        template <typename... Params>
        TypeX* operator()(Params... params)
        {
            auto& memorySystem = GetGlobalMemorySystem();

            IAllocator* pAllocator = memorySystem.GetAllocator();

            const size_t memorySize = sizeof(TypeX) + sizeof(MemoryMetaData);

            auto pMemory = static_cast<char*>(pAllocator->Alloc(memorySize));
            MemoryMetaData* block = new(pMemory)MemoryMetaData;
            pMemory += sizeof(MemoryMetaData);
            TypeX* pObject = new (pMemory)TypeX((params)...);

            block->objectSize = sizeof(TypeX);
            block->arrayCount = 1;
            block->fileName = metaData.filename;
            block->line = metaData.line;
            block->functionName = metaData.function;
            block->pAllocator = pAllocator;
            block->pointer = pObject;
            block->typeData = typeid(TypeX).name();
            pMemory += sizeof(MemoryMetaData);
            
            if (memorySystem.IsEnableMemoryLeak())
            {
                memorySystem.RegisterMemoryMetaData(block);
            }

            return pObject;
        }
    };

    /**
    * \brief メモリを開放する
    * \param ptr メモリを開放するポインタ
    */
    template <typename TypeX>
    void DeleteMemory(TypeX*& ptr)
    {

        auto& memorySystem = GetGlobalMemorySystem();

        IAllocator* pAllocator = memorySystem.GetAllocator();

        MemoryMetaData& meta = GetMemoryMetaDeta(ptr);

        void * pMemoryHead = &meta;


        TypeX* pCurrent = ptr;
        for (unsigned i = 0; i<meta.arrayCount; ++i)
        {
            pCurrent[i].~TypeX();
        }

        if (memorySystem.IsEnableMemoryLeak())
            memorySystem.RemoveMemoryMetaData(&meta);

        pAllocator->Free(pMemoryHead);
        ptr = nullptr;
    }

    /**
    * \brief 配列のメモリを確保する
    */
    template <typename TypeX>
    TypeX* AllocArray(const int line,
        const char* filename,
        const char* functionname,
        const int itemCount)
    {
        auto& memorySystem = GetGlobalMemorySystem();

        IAllocator* pAllocator = GetGlobalMemorySystem().GetAllocator();

        const size_t memorySize = sizeof(TypeX) * itemCount + sizeof(MemoryMetaData);

        auto pMemory = static_cast<char*>(pAllocator->Alloc(memorySize));

        MemoryMetaData* block = new(pMemory)MemoryMetaData;

        block->objectSize = sizeof(TypeX);
        block->arrayCount = itemCount;

        pMemory += sizeof(MemoryMetaData);

        TypeX* pCurrent = reinterpret_cast<TypeX*>(pMemory);

        for (int i = 0; i<itemCount; ++i)
        {
            new (pCurrent)TypeX;
            ++pCurrent;
        }

        // ! メタ情報を埋め込んでおく
        if (memorySystem.IsEnableMemoryLeak())
        {
            block->line = line;
            block->fileName = filename;
            block->functionName = functionname;
            block->pAllocator = pAllocator;

            block->pointer = pMemory;
            block->typeData = typeid(TypeX).name();
            memorySystem.RegisterMemoryMetaData(block);
        }
        return reinterpret_cast<TypeX*>(pMemory);
    }
}