#pragma once

/**
* \brief ���������m�ۂ����Ƃ��̃w�b�_�f�[�^
*/
struct MemoryMetaData
{
    size_t      objectSize;     //! �m�ۂ����I�u�W�F�N�g��1������T�C�Y
    unsigned    arrayCount;     //! �m�ۂ����I�u�W�F�N�g�̐�

    MemoryMetaData * pPrevBlock;
    MemoryMetaData * pNextBlock;

    int         line;           //! �\�[�X�R�[�h���ł̍s�ԍ�
    const char* fileName;       //! �\�[�X�R�[�h�̃t�@�C����
    const char* functionName;   //! �֐���
    const char* typeData;       //! �^�C�v���

    void*		pointer;		//! �擪�|�C���^���
    IAllocator* pAllocator;		//! �A���P�[�^���

};

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
    * \param metadata
    */
    void RegisterMemoryMetaData(MemoryMetaData* metadata);
    
    /**
    * \brief ���������J������ۂɓo�^�������^�f�[�^���폜����
    * \param pointer
    */
    void RemoveMemoryMetaData(MemoryMetaData * metadata);
    
    unsigned GetAllocMemoryCount()const;

    void DumpLog();

private:
    bool _isLeakChek;
    IAllocator* _pAllocator;
    MemoryMetaData* _pHeadMetaData;
    MemoryMetaData* _pCurrentMetaData;

};