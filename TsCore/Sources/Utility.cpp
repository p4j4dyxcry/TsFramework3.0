#include "pch.h"
#include <fstream>
#include "Utility.h"

TS::ManagedArray<unsigned char> TS::ReadBinary(const char * filepath)
{
    std::ifstream ifs(filepath, std::ifstream::in | std::ifstream::binary);

    if (ifs.fail())
    {
        Error::Make(_T("File Read error")).Log();
        return TS::ManagedArray<unsigned char>(0);
    }

    const unsigned begin = static_cast<unsigned>(ifs.tellg());
    ifs.seekg(0, ifs.end);

    const unsigned end = static_cast<unsigned>(ifs.tellg());
    size_t binarySize = (end - begin);
    ifs.clear();
    ifs.seekg(0, ifs.beg);

    auto managed_array = ManagedArray<unsigned char>(binarySize);
    ifs.read(reinterpret_cast<char*>(&managed_array[0]), binarySize);

    return managed_array;
}

TS::ManagedArray<unsigned char> TS::ReadBinary(const wchar_t * filepath)
{
    std::ifstream ifs(filepath, std::ifstream::in | std::ifstream::binary);

    if (ifs.fail())
    {
        Error::Make(_T("File Read error")).Log();
        return TS::ManagedArray<unsigned char>(0);
    }

    const unsigned begin = static_cast<unsigned>(ifs.tellg());
    ifs.seekg(0, ifs.end);

    const unsigned end = static_cast<unsigned>(ifs.tellg());
    size_t binarySize = (end - begin);
    ifs.clear();
    ifs.seekg(0, ifs.beg);

    auto managed_array = ManagedArray<unsigned char>(binarySize);
    ifs.read(reinterpret_cast<char*>(&managed_array[0]), binarySize);

    return managed_array;
}