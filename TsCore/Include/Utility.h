#pragma once

#include "ManagedArray.h"

namespace TS
{
    ManagedArray<unsigned char> ReadBinary(const char   * filepath);
    ManagedArray<unsigned char> ReadBinary(const wchar_t * filepath);
}