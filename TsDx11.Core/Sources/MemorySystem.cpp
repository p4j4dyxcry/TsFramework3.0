#include "pch.h"
#include "Common.h"
#include "IAllocator.h"
#include "MemorySystem.h"
#include "New.h"
#include "ManagedArray.h"
#include "String.h"


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

unsigned MemorySystem::GetAllocMemoryCount()const
{
    unsigned count = 0;

    for (auto p = _pHeadMetaData;
        p != nullptr;
        p = p->pNextBlock, ++count)
    {
    }
    return count;
}
