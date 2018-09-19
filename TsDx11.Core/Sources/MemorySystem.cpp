#include "pch.h"
#include "Common.h"
#include "IAllocator.h"
#include "MemorySystem.h"

namespace TS
{
    MemorySystem::MemorySystem(IAllocator * pAllocator, bool _isMemoryLeakCheck) :
        _isLeakChek(_isMemoryLeakCheck),
        _pAllocator(pAllocator),
        _pHeadMetaData(nullptr),
        _pCurrentMetaData(nullptr)
    {
    }

    bool MemorySystem::IsEnableMemoryLeak() const
    {
        return _isLeakChek;
    }

    IAllocator* MemorySystem::GetAllocator()const
    {
        return _pAllocator;
    }

    void MemorySystem::RegisterMemoryMetaData(MemoryMetaData* pMetaData)
    {
        pMetaData->line = _debugInfo.line;
        pMetaData->fileName = _debugInfo.filename;
        pMetaData->functionName = _debugInfo.function;
        _debugInfo = DebugInfo();

        if (_pHeadMetaData == nullptr)
        {
            _pHeadMetaData = pMetaData;
        }

        if (_pCurrentMetaData != nullptr)
        {
            _pCurrentMetaData->pNextBlock = pMetaData;
            pMetaData->pPrevBlock = _pCurrentMetaData;
        }
        else
        {
            pMetaData->pPrevBlock = nullptr;
        }

        _pCurrentMetaData = pMetaData;
        _pCurrentMetaData->pNextBlock = nullptr;
    }

    void MemorySystem::RemoveMemoryMetaData(MemoryMetaData* pMetaData)
    {
        if (pMetaData->pPrevBlock != nullptr)
            pMetaData->pPrevBlock->pNextBlock = pMetaData->pNextBlock;
        else
            _pHeadMetaData = pMetaData->pNextBlock;

        if (pMetaData->pNextBlock != nullptr)
            pMetaData->pNextBlock->pPrevBlock = pMetaData->pPrevBlock;
        else
            _pCurrentMetaData = pMetaData->pPrevBlock;
    }

    MemorySystem& MemorySystem::SetDebugInfo(const DebugInfo& debugInfo)
    {
        _debugInfo = debugInfo;
        return *this;
    }

    unsigned MemorySystem::GetAllocatedChunkCount()const
    {
        unsigned count = 0;

        for (auto p = _pHeadMetaData;
            p != nullptr;
            p = p->pNextBlock, ++count)
        {
        }
        return count;
    }
}
