#pragma once

namespace TS
{
    InputElementDesc MakeElementDescFloat(const char* name, unsigned dataCount, unsigned semanticIndex);
    InputElementDesc MakeElementDescUInt(const char* name, unsigned dataCount, unsigned semanticIndex);
    InputElementDesc MakeElementDescUnorm(const char* name, unsigned dataCount, unsigned semanticIndex);
    InputElementDesc MakeElementDescSint(const char* name, unsigned dataCount, unsigned semanticIndex);

    InputElementDesc ElementDescPosition3f(unsigned id = 0);
    InputElementDesc ElementDescPosition4f(unsigned id = 0);
    InputElementDesc ElementDescUv2f(unsigned id = 0);
    InputElementDesc ElementDescColor3f(unsigned id = 0);
    InputElementDesc ElementDescColor4f(unsigned id = 0);
    InputElementDesc ElementDescColor4u(unsigned id = 0);
    InputElementDesc ElementDescNormal3f(unsigned id = 0);
    InputElementDesc ElementDescNormal4f(unsigned id = 0);
    InputElementDesc ElementDescTangent4f(unsigned id = 0);
    InputElementDesc ElementDescBinormal4f(unsigned id = 0);
    InputElementDesc ElementDescBlendIndices4u(unsigned id = 0);
    InputElementDesc ElementDescBlendWeightf(unsigned id = 0);
    InputElementDesc ElementDescVertexId();
    InputElementDesc ElementDescInstanceId();
    InputElementDesc ElementDescPrimitiveId();

    MemoryManagedArray<InputElementDesc> MakeInputLayoutDescFromMemory(Binary& binary);
}