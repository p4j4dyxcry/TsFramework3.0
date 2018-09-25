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
     * \brief アロケータ
     */
    class Allocator : public IAllocator
    {
    public:
        /**
         * \brief コンストラクタ
         * \param size 最大メモリ(byte)
         */
        Allocator(size_t size);

        virtual ~Allocator();


        void* Alloc(size_t size) override;

        void Free(void* pointer) override;

    private:
        //! 空きチャンクを探す
        Chunk* FindEmptyChunk(size_t size);

        //! チャンクから必要なメモリを確保し新しいサブチャンクを作成する
        void MakeNextChunk(Chunk* chunk, size_t sz);

        //! チャンクの解放
        void FreeChunk(Chunk* chunk);

        // ! 未使用チャンクの結合
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