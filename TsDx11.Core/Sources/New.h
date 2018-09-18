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
    * \brief �������̃��^�f�[�^���擾���܂��B
    * \param pointer ���������J������|�C���^
    */
    inline MemoryMetaData& GetMemoryMetaDeta(void* pointer)
    {
        return *reinterpret_cast<MemoryMetaData*>(static_cast<char*>(pointer) - sizeof(MemoryMetaData));
    }

    /**
    * \brief ���^���𖄂ߍ��݂ϒ��R���X�g���N�^���ĂԎd�g��
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

        //���^���̖��ߍ���
        FuncData metaData;
        MetaDataIncludeConstructor(const int line,
            const char* filename,
            const char* functionname)
        {
            metaData.line = line;
            metaData.filename = filename;
            metaData.function = functionname;
        }

        //! �Ώۂ̃��������m�ۂ��R���X�g���N�^���Ăяo��
        /**
        * \brief�Ώۂ̃��������m�ۂ��R���X�g���N�^���Ăяo��
        * \param params �쐬����N���X�̃R���X�g���N�^����
        * \return �쐬���ꂽ�I�u�W�F�N�g
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
    * \brief ���������J������
    * \param ptr ���������J������|�C���^
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
    * \brief �z��̃��������m�ۂ���
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

        // ! ���^���𖄂ߍ���ł���
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