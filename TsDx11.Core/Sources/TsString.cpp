#include "pch.h"
#include "Common.h"
#include "MemoryManagedArray.h"
#include "TsSring.h"

//! �{�C���[-���[�A�@�ɂ�镶���񌟍��A���S���Y��
inline int BoyerMooreStringSearch(const TS::TsChar* origin, int origin_length, const TS::TsChar * pattern, int pattern_length)
{
    if (pattern_length > origin_length)
        return -1;

#ifdef UNICODE // 16 byte ( ushort max)
    static int skipTable[65535];
#else          // 8  byte ( uchar  max)
    static int skipTable[255];
#endif        
    for (int& cell : skipTable)
        cell = pattern_length - 1;

    for (int i = 0; i < pattern_length; ++i)
        skipTable[static_cast<int>(pattern[i])] = pattern_length - i - 1;

    int j;
    int idx = j = pattern_length - 1;

    while (true)
    {
        if (idx >= static_cast<int>(origin_length)) break;
        if (j < 0) break;

        if (origin[idx] != pattern[j])
        {
            idx += max(skipTable[static_cast <int>(origin[idx])], pattern_length - j);
            j = pattern_length - 1;
        }
        else
        {
            j--;
            idx--;
        }
    }
    if (idx == origin_length)
        return -1;

    return idx;
}

TS::TsString::TsString(const TsChar* str)
{
    *this = str;
}

TS::TsString& TS::TsString::operator=(const TsChar* str)
{
    Release();
    _size = Length(str);

    _data = new TsChar[_size];
    unsigned i = 0;
    for (; i < _size; ++i)
        _data[i] = str[i];

    _refarenceConter = new RefCounter();
    _refarenceConter->AddRef();

    return *this;
}

TS::TsString TS::TsString::operator+(const TsChar* str) const
{
    const unsigned str_size = Length(str);
    const unsigned sz = (_size + str_size) - 1;

    TsString result(sz);
    unsigned i = 0;
    for (; _data[i] != '\0'; ++i)
        result[i] = _data[i];

    for (unsigned j = 0; j < str_size; ++j)
        result[i + j] = str[j];

    return result;
}

TS::TsString& TS::TsString::operator+=(const TsChar* str)
{
    *this = *this + str;
    return *this;
}

TS::TsString TS::TsString::SubString(unsigned pos, unsigned npos) const
{
    if (npos == 0)
        npos = static_cast<unsigned>(_size) - pos - 1;

    TsString result(npos + 1);

    unsigned i = 0;
    for (; i < npos; ++i)
        result[i] = _data[pos + i];
    result[i] = '\0';

    return result;
}

int TS::TsString::Find(const TsChar* _pattern) const
{
    const int pattern_length = Length(_pattern) - 1;

    return BoyerMooreStringSearch(_data, _size, _pattern, pattern_length);
}

TS::TsString TS::TsString::Replace(const TsChar original, const TsChar _new) const
{
    TsString result(_size);
    for (unsigned i = 0; i < _size; ++i)
    {
        if (_data[i] == original)
            result[i] = _new;
        else
            result[i] = _data[i];
    }

    return result;
}

TS::TsString TS::TsString::Replace(unsigned start, unsigned size, const TsString& str) const
{
    if (start == 0)
        return str + SubString(size, _size - size);

    return SubString(0, start + 1) + str + SubString(start + size, _size - start - size);
}

TS::TsString TS::TsString::Replace(const TsChar* original, const TsChar* _new) const
{
    const int sz = Length(original);

    int _index = Find(original);

    if (_index == -1)
        return *this;

    auto result = *this;
    while (_index != -1)
    {
        result = result.Replace(_index, sz, _new);
        _index = result.Find(original);
    }
    return result;
}

bool TS::TsString::Contain(const TsChar* str) const
{
    return Find(str) != -1;
}

size_t TS::TsString::Length(const TsChar* str)
{
    size_t sz = 0;
    while (str[sz++] != '\0')
    {
        //! empty loop
    }
    return sz;
}

bool TS::TsString::operator==(const TsString& string) const
{
    if (_size != string._size)
        return false;

    for (size_t i = 0; i < _size; ++i)
    {
        if (_data[i] != string[i])
            return false;
    }
    return true;
}

bool TS::TsString::operator!=(const TsString& string) const
{
    return !(*this == string);
}

inline bool TS::TsString::IsNullOrEmpty() const
{
    return _data == nullptr || _size == 0;
}

TS::TsString TS::TsString::Format(const TsChar* pcFormat, ...)
{
    va_list valist; 
    
    va_start(valist, pcFormat);
    const size_t _maxBuffer = 65535;
    static TsChar table[_maxBuffer]{};

#ifdef UNICODE
    vswprintf_s(table, _maxBuffer, pcFormat, valist);
#else
    vsprintf_s(table, _maxBuffer, pcFormat, valist);
#endif
    va_end(valist);
    return table;
}
