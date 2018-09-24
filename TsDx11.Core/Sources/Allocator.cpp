#include "pch.h"
#include "Common.h"
#include "IAllocator.h"
#include "Allocator.h"

namespace TS
{
	size_t calc_remaining(Chunk * chunk_addres, 
						  void * pMemoryHead, 
						  size_t memory_size)
	{
		unsigned char * base_address = (unsigned char *)chunk_addres->Memory();
		unsigned char * end_addres = (unsigned char *)pMemoryHead + memory_size;

		return (size_t)(end_addres - base_address);
	}
	
	size_t clack_chunkSize(Chunk* chunk	, void * pMemoryHead,size_t memory_size)
	{
		if (chunk->Next != nullptr)
		{
			return ((unsigned char*)chunk->Next - (unsigned char*)chunk->Memory());
		}
		else
			return calc_remaining(chunk, pMemoryHead, memory_size);
	}

    Allocator::Allocator(size_t size)
    {
        _pMemory = static_cast<unsigned char *>(malloc(size));
		_memorySize = size;

		_headChunk = reinterpret_cast<Chunk*>(_pMemory);
        _headChunk->Next = nullptr;
        _headChunk->Prev = nullptr;
        _headChunk->Using = false;
		_headChunk->Size = calc_remaining(_headChunk, _pMemory, _memorySize);

        _currentChunk = _headChunk;
    }

    Allocator::~Allocator()
    {
        free(_pMemory);
    }

    void* Allocator::Alloc(size_t size)
    {
        Chunk* chunk = FindEmptyChunk(size + sizeof(Chunk));
        Error::Assert(chunk != nullptr, L"空きメモリが見つかりません。");
        MakeNextChunk(chunk, size);
        return chunk->Memory();
    }

    void Allocator::Free(void* pointer)
    {
        Chunk* usingChunk = ToHeader(pointer);
        Error::Assert(usingChunk != nullptr, L"メモリの解放に失敗しました。");
        FreeChunk(usingChunk);
    }

    Chunk* Allocator::FindEmptyChunk(size_t size)
    {
        // カレントから末尾まで探す
        Chunk* pChunk = _currentChunk;
        while (_currentChunk)
        {
            if (!_currentChunk->Using && _currentChunk->Size >= size)
            {
                return _currentChunk;
            }

            _currentChunk = _currentChunk->Next;
        }

        // 先頭からカレントまで探す
        _currentChunk = _headChunk;
        while (_currentChunk != pChunk)
        {
            if (!_currentChunk->Using && _currentChunk->Size >= size)
            {
                return _currentChunk;
            }

            _currentChunk = _currentChunk->Next;
        }
        return nullptr;
    }

    void Allocator::MakeNextChunk(Chunk* chunk, size_t sz)
    {
        //次の空きアドレスは現在アドレス + 実メモリ領域までのアドレス(sizeof(Chunk)) + 占有量(size) 
        void * nextChunkAddress = reinterpret_cast<unsigned char*>(chunk) + sizeof(Chunk) + sz;
        Chunk* newChunk = reinterpret_cast<Chunk*>(nextChunkAddress);

		Chunk* nextChunk = chunk->Next;

        newChunk->Next = nextChunk;
        newChunk->Prev = chunk;
        newChunk->Using = false;

        chunk->Next = newChunk;
        chunk->Using = true;

        if (nextChunk != nullptr)
        {
            newChunk->Next = nextChunk;
            nextChunk->Prev = newChunk;
        }
		chunk->Size    = clack_chunkSize(chunk, _pMemory, _memorySize);
		newChunk->Size = clack_chunkSize(newChunk, _pMemory, _memorySize);
    }

    void Allocator::FreeChunk(Chunk* chunk)
    {
        chunk->Using = false;
        MergeChunk(chunk);
    }

    void Allocator::MergeChunk(Chunk* chunk)
    {
        Chunk* prevChunk = chunk->Prev;
        Chunk* nextChunk = chunk->Next;

        if(prevChunk == nullptr || prevChunk->Using)
            prevChunk = chunk;

        //! 一つ後のチャンクが空ならマージする
        if (nextChunk != nullptr && !nextChunk->Using)
        {
            nextChunk = nextChunk->Next;
            prevChunk->Next = nextChunk;
        }
        else
            prevChunk->Next = nextChunk;

		if (nextChunk != nullptr)
		{
			nextChunk->Prev = prevChunk;
		}

        _currentChunk = prevChunk;
		_currentChunk->Size = clack_chunkSize(_currentChunk, _pMemory, _memorySize);
    }

    Chunk* Allocator::ToHeader(void* pointer)
    {
        //! アドレス - Chunk構造体のサイズで　Chunk構造体へのアドレスを求める
        return reinterpret_cast<Chunk*>(static_cast<char*>(pointer) - sizeof(Chunk));
    }
}
