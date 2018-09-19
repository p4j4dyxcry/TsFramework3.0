#include "pch.h"
#include "Common.h"
#include "IAllocator.h"
#include "Allocator.h"

namespace TS
{
    Allocator::Allocator(size_t size)
    {
        _pMemory = static_cast<unsigned char *>(malloc(size));
        _headChunk = reinterpret_cast<Chunk*>(_pMemory);
        _headChunk->Size = size - sizeof(Chunk);
        _headChunk->Next = nullptr;
        _headChunk->Prev = nullptr;
        _headChunk->Using = false;
        _currentChunk = _headChunk;
    }

    Allocator::~Allocator()
    {
        free(_pMemory);
    }

    void* Allocator::Alloc(size_t size)
    {
        Chunk* chunk = FindEmptyChunk(size);
        Error::Assert(chunk != nullptr, L"�󂫃�������������܂���B");
        MakeNextChunk(chunk, size);
        return chunk->Memory();
    }

    void Allocator::Free(void* pointer)
    {
        Chunk* usingChunk = ToHeader(pointer);
        Error::Assert(usingChunk != nullptr, L"�������̉���Ɏ��s���܂����B");
        FreeChunk(usingChunk);
    }

    Chunk* Allocator::FindEmptyChunk(size_t size)
    {
        // �J�����g���疖���܂ŒT��
        Chunk* pChunk = _currentChunk;
        while (_currentChunk)
        {
            if (!_currentChunk->Using && _currentChunk->Size >= size)
            {
                return _currentChunk;
            }

            _currentChunk = _currentChunk->Next;
        }

        // �擪����J�����g�܂ŒT��
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
        Chunk* nextChunk = chunk->Next;

        //���̋󂫃A�h���X�͌��݃A�h���X + ���������̈�܂ł̃A�h���X(sizeof(Chunk)) + ��L��(size) 
        void * nextChunkAddress = reinterpret_cast<unsigned char*>(chunk) + sizeof(Chunk) + sz;
        Chunk* newChunk = reinterpret_cast<Chunk*>(nextChunkAddress);

        newChunk->Next = nullptr;
        newChunk->Prev = chunk;
        newChunk->Size = chunk->Size - sz - sizeof(Chunk);
        newChunk->Using = false;

        chunk->Next = newChunk;
        chunk->Using = true;
        chunk->Size = sz;

        if (nextChunk != nullptr)
        {
            newChunk->Next = nextChunk;
            nextChunk->Prev = newChunk;
        }
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

        //! ��O�̃`�����N����Ȃ�}�[�W����
        if (prevChunk != nullptr && !prevChunk->Using)
            prevChunk->Size += chunk->Size + sizeof(Chunk);
        else
            prevChunk = chunk;

        //! ���̃`�����N����Ȃ�}�[�W����
        if (nextChunk != nullptr && !nextChunk->Using)
        {
            prevChunk->Size += nextChunk->Size + sizeof(Chunk);

            nextChunk = nextChunk->Next;
            prevChunk->Next = nextChunk;
        }
        else
            prevChunk->Next = nextChunk;

        if (nextChunk != nullptr)
            nextChunk->Prev = prevChunk;

        _currentChunk = prevChunk;
    }

    Chunk* Allocator::ToHeader(void* pointer)
    {
        //! �A�h���X - Chunk�\���̂̃T�C�Y�Ł@Chunk�\���̂ւ̃A�h���X�����߂�
        return reinterpret_cast<Chunk*>(static_cast<char*>(pointer) - sizeof(Chunk));
    }
}
