#pragma once
//! �{�C���[-���[�A�@�ɂ�镶���񌟍��A���S���Y��
namespace TS
{
    template<typename T>
    inline int BoyerMooreStringSearch(const T* origin, int origin_length, const T * pattern, int pattern_length)
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

    template <typename T>
    String<T>::String(const T* str)
    {
        *this = str;
    }

    template <typename T>
    String<T>::String(const String<T>& ref) : TS::String<T>(
        ref._data, ref._size, ref._refarenceConter)
    {
    }

    template <typename T>
    String<T>::String(const String<T>&& ref) noexcept : TS::String<T>(
        ref._data, ref._size, ref._refarenceConter)
    {
    }

    template<typename T>
    TS::String<T>& TS::String<T>::operator=(const T* str)
    {
        this->Release();
        this->_size = Length(str);

        this->_data = TS_NEWARRAY(T, this->_size);
        unsigned i = 0;
        for (; i < this->_size; ++i)
            this->_data[i] = str[i];

        this->AddRef(nullptr);

        return *this;
    }

    template<typename T>
    TS::String<T> TS::String<T>::operator+(const T* str) const
    {
        const unsigned str_size =  Length(str);
        const unsigned sz = (this->_size + str_size) - 1;

        String result(sz);
        unsigned i = 0;
        for (; this->_data[i] != '\0'; ++i)
            result[i] = this->_data[i];

        for (unsigned j = 0; j < str_size; ++j)
            result[i + j] = str[j];

        return result;
    }

    template<typename T>
    TS::String<T>& TS::String<T>::operator+=(const T* str)
    {
        *this = *this + str;
        return *this;
    }

    template<typename T>
    inline String<T>& String<T>::operator=(const String<T>& ref)
    {
        this->Release();

        this->_data = ref._data;
        this->_size = ref._size;

        this->AddRef(ref._refarenceConter);
        return *this;
    }

    template<typename T>
    inline String<T>& String<T>::operator=(String<T>&& ref) noexcept
    {
        this->Release();

        this->_data = ref._data;
        this->_size = ref._size;

        this->AddRef(ref._refarenceConter);
        return *this;
    }

    template<typename T>
    TS::String<T> TS::String<T>::SubString(unsigned pos, unsigned npos) const
    {
        if (npos == 0)
            npos = static_cast<unsigned>(this->_size) - pos - 1;

        String result(npos + 1);

        unsigned i = 0;
        for (; i < npos; ++i)
            result[i] = this->_data[pos + i];
        result[i] = '\0';

        return result;
    }

    template<typename T>
    int TS::String<T>::Find(const T* _pattern) const
    {
        const int pattern_length = Length(_pattern) - 1;

        return BoyerMooreStringSearch(this->_data, this->_size, _pattern, pattern_length);
    }

    template<typename T>
    TS::String<T> TS::String<T>::Replace(const T original, const T _new) const
    {
        String result(this->_size);
        for (unsigned i = 0; i < this->_size; ++i)
        {
            if (this->_data[i] == original)
                result[i] = _new;
            else
                result[i] = this->_data[i];
        }

        return result;
    }

    template<typename T>
    TS::String<T> TS::String<T>::Replace(unsigned start, unsigned size, const String<T>& str) const
    {
        if (start == 0)
            return str + SubString(size, this->_size - size);

        return SubString(0, start + 1) + str + SubString(start + size, this->_size - start - size);
    }

    template<typename T>
    TS::String<T> TS::String<T>::Replace(const T* original, const T* _new) const
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

    template<typename T>
    bool TS::String<T>::Contain(const T* str) const
    {
        return Find(str) != -1;
    }

    template<typename T>
    size_t TS::String<T>::Length(const T* str)
    {
        size_t sz = 0;
        while (str[sz++] != '\0')
        {
            //! empty loop
        }
        return sz;
    }

    template<typename T>
    bool TS::String<T>::operator==(const String<T>& string) const
    {
        if (this->_size != string._size)
            return false;

        for (size_t i = 0; i < this->_size; ++i)
        {
            if (this->_data[i] != string[i])
                return false;
        }
        return true;
    }

    template<typename T>
    bool TS::String<T>::operator!=(const String<T>& string) const
    {
        return !(*this == string);
    }

    template<typename T>
    inline bool TS::String<T>::IsNullOrEmpty() const
    {
        return this->_data == nullptr || this->_size == 0;
    }

    template<>
    inline TS::String<char> TS::String<char>::Format(const char* pcFormat, ...)
    {
        va_list valist;

        va_start(valist, pcFormat);
        const size_t _maxBuffer = 65535;
        static char table[_maxBuffer]{};

        vsprintf_s(table, _maxBuffer, pcFormat, valist);
        va_end(valist);
        return table;
    }

    template<>
    inline TS::String<wchar_t> TS::String<wchar_t>::Format(const wchar_t* pcFormat, ...)
    {
        va_list valist;

        va_start(valist, pcFormat);
        const size_t _maxBuffer = 65535;
        static wchar_t table[_maxBuffer]{};

        vswprintf_s(table, _maxBuffer, pcFormat, valist);
        va_end(valist);
        return table;
    }
}