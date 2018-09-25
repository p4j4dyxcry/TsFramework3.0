#pragma once
namespace TS
{
    struct Chunk
    {
        Chunk*  Next;
        Chunk*  Prev;
        size_t  Size;
        bool    Using;
        void *  Memory()
        {
            return reinterpret_cast<unsigned char*>(this) + sizeof(Chunk);
        }
    };

    /**
     * \brief �A���P�[�^
     */
    class Allocator : public IAllocator
    {
    public:
        /**
         * \brief �R���X�g���N�^
         * \param size �ő僁����(byte)
         */
        Allocator(size_t size);

        virtual ~Allocator();


        void* Alloc(size_t size) override;

        void Free(void* pointer) override;

    private:
        //! �󂫃`�����N��T��
        Chunk* FindEmptyChunk(size_t size);

        //! �`�����N����K�v�ȃ��������m�ۂ��V�����T�u�`�����N���쐬����
        void MakeNextChunk(Chunk* chunk, size_t sz);

        //! �`�����N�̉��
        void FreeChunk(Chunk* chunk);

        // ! ���g�p�`�����N�̌���
        void MergeChunk(Chunk* chunk);

        static Chunk* ToHeader(void* pointer);
    private:
        unsigned char * _pMemory;
        size_t			_memorySize;
        Chunk * _currentChunk;
        Chunk * _headChunk;

    };
}
;