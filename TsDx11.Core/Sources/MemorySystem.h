#pragma once

namespace TS
{
    /**
    * \brief
    */
    struct DebugInfo
    {
        int line;
        const char* filename;
        const char* function;

        DebugInfo(int __line__ = 0, const char * __filename__ = "", const char * __functionname__ = "")
        {
            line = __line__;
            filename = __filename__;
            function = __functionname__;
        }
    };

    /**
    * \brief ���������m�ۂ����Ƃ��̃w�b�_�f�[�^
    */
    struct MemoryMetaData
    {
        size_t      objectSize;     //! �m�ۂ����I�u�W�F�N�g��1������T�C�Y
        size_t      arrayCount;     //! �m�ۂ����I�u�W�F�N�g�̐�

        MemoryMetaData * pPrevBlock;
        MemoryMetaData * pNextBlock;

        int         line;           //! �\�[�X�R�[�h���ł̍s�ԍ�
        const char* fileName;       //! �\�[�X�R�[�h�̃t�@�C����
        const char* functionName;   //! �֐���
        const char* typeData;       //! �^�C�v���
    };
    
    /**
    * \brief �������̃��^�f�[�^���擾���܂��B
    * \param pointer ���������J������|�C���^
    */
    inline MemoryMetaData& GetMemoryMetaDeta(void* pointer)
    {
        return *reinterpret_cast<MemoryMetaData*>(static_cast<char*>(pointer) - sizeof(MemoryMetaData));
    }

    class MemorySystem
    {
    public:
        MemorySystem(IAllocator * pAllocator, bool _isMemoryLeakCheck);

        /**
        * \brief ���������[�N�`�F�b�N���L�����擾����
        * \return
        */
        bool IsEnableMemoryLeak() const;

        /**
        * \brief �f�t�H���g�A���P�[�^���擾����
        * \return
        */
        IAllocator* GetAllocator()const;

        /**
        * \brief ���������m�ۂ����ۂ̏���o�^����
        * \param pMetaData
        */
        void RegisterMemoryMetaData(MemoryMetaData* pMetaData);

        /**
        * \brief ���������J������ۂɓo�^�������^�f�[�^���폜����
        * \param pMetaData
        */
        void RemoveMemoryMetaData(MemoryMetaData * pMetaData);

        /**
        * \brief �f�o�b�O����ݒ肷��A Alloc �̑O�ɌĂяo��
        * \param debugInfo
        */
        MemorySystem& SetDebugInfo(const DebugInfo& debugInfo);

        template<typename T, typename... Params>
        T* Alloc(Params ... params);

        /**
        * \brief �z��̃��������m�ۂ���
        */
        template<typename T>
        T* AllocArray(size_t itemCount = 0);

        /**
        * \brief ���������J������
        * \param ptr ���������J������|�C���^
        */
        template <typename T>
        void Delete(T*& ptr);

        /**
         * \brief ���݊m�ۂ���Ă��郁�����`�����N�̐����擾����
         * \return 
         */
        unsigned GetAllocatedChunkCount()const;
    private:
        bool _isLeakChek;
        DebugInfo   _debugInfo;
        IAllocator* _pAllocator;
        MemoryMetaData* _pHeadMetaData;
        MemoryMetaData* _pCurrentMetaData;
    };
}

#include "MemorySystem.hpp"
