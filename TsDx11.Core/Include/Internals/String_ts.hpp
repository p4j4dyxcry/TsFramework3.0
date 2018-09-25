#pragma once
namespace TS
{
    template <typename T>
    int BoyerMooreStringSearch(const T * original , int original_length, const T * pattern, int pattern_length)
    {
        int original_current_index = pattern_length - 1;		

        static int skip_table[65535]{};

        for (int i = 0; i < pattern_length; i++)
            skip_table[static_cast<int>(pattern[i])] = (pattern_length - 1) - i;

        while(original_current_index < original_length)
        {
            int pattern_current_index = pattern_length - 1;

            while (0 <= pattern_current_index)
            {
                if (original[original_current_index] ==
                    pattern [pattern_current_index])
                {
                    --original_current_index;
                    --pattern_current_index;
                }
                else break;
            }
            if (pattern_current_index < 0)
                return original_current_index + 1;

            original_current_index +=
                max(skip_table[static_cast<int>(pattern[pattern_current_index])],
                    pattern_length - pattern_current_index);

        }
        //! not found
        return -1;
    }

    template <typename T>
    int ReversBoyerMooreStringSearch(const T * original, int original_length, const T * pattern, int pattern_length)
    {
        int original_current_index = (original_length - 2) - (pattern_length - 1);	

        static int skip_table[65535]{};

        for (int i = 0; i < pattern_length; i++)
            skip_table[static_cast<int>(pattern[i])] = i + 1;

        while ( 0 <= original_current_index)
        {
            int pattern_current_index = 0;

            for (; pattern_current_index < pattern_length;)
            {
                if (original[original_current_index] ==
                    pattern[pattern_current_index])
                {
                    ++original_current_index;
                    ++pattern_current_index;
                }
                else break;
            }
            if (pattern_current_index == pattern_length)
                return original_current_index - pattern_length;

            original_current_index -=
                max(skip_table[static_cast<int>(pattern[pattern_current_index])],
                    pattern_current_index + 1);
        }
        //! not found
        return -1;
    }


    template <typename T>
    String<T>::String(const T* str):String<T>()
    {
        copy_all_internal(str, StringLength(str));
    }

    template <typename T>
    String<T>::String(const String<T>& ref) : TS::String<T>()
    {
        if (this != &ref)
            copy_all_internal(ref._data, ref._size);
    }

    template <typename T>
    String<T>::String(const String<T>&& ref) noexcept : TS::String<T>()
    {
        if(this != &ref)
            copy_all_internal(ref._data, ref._size);
    }

    template<typename T>
    TS::String<T>& TS::String<T>::operator=(const T* str)
    {
        return copy_all_internal(str, StringLength(str));
    }

    template<typename T>
    TS::String<T> TS::String<T>::operator+(const T* str) const
    {
        const unsigned str_size =  StringLength(str);
        const unsigned sz = (_size + str_size) - 1;

        String result;
        result.Resize(sz);
        unsigned i = 0;
        for (; _data[i] != '\0'; ++i)
            result[i] = _data[i];

        for (unsigned j = 0; j < str_size; ++j)
            result[i + j] = str[j];

        return result;
    }

    template<typename T>
    TS::String<T>& TS::String<T>::operator+=(const T* str)
    {
        return AddRange(str);
    }

    template<typename T>
    inline String<T>& String<T>::operator=(const String<T>& ref)
    {
        if (this != &ref)
            return copy_all_internal(ref._data,ref._size);
        return *this;
    }

    template<typename T>
    inline String<T>& String<T>::operator=(String<T>&& ref) noexcept
    {
        if(this != &ref)
            return copy_all_internal(ref._data, ref._size);
        return *this;
    }


    template<typename T>
    TS::String<T> TS::String<T>::SubString(unsigned pos, unsigned npos) const
    {
        if (npos == 0)
            npos = static_cast<unsigned>(_size) - pos - 1;

        String result;
        result.Resize(npos + 1);

        unsigned i = 0;
        for (; i < npos; ++i)
            result[i] = _data[pos + i];
        result[i] = '\0';

        return result;
    }

    template<typename T>
    int TS::String<T>::Find(const T* _pattern) const
    {
        const int pattern_length = StringLength(_pattern) - 1;

        return BoyerMooreStringSearch(this->_data, this->_size, _pattern, pattern_length);
    }

    template<typename T>
    int TS::String<T>::Find(const T& _pattern) const
    {
        for (size_t i = 0; i < _size; ++i)
            if (_data[i] == _pattern)
                return i;
        return -1;
    }

    template<typename T>
    int TS::String<T>::Rfind(const T* _pattern) const
    {
        const int pattern_length = StringLength(_pattern) - 1;

        return ReversBoyerMooreStringSearch(this->_data, this->_size, _pattern, pattern_length);
    }

    template<typename T>
    int TS::String<T>::Rfind(const T& _pattern) const
    {
        for (size_t i = 0; i < _size; ++i)
            if (_data[(_size - 1) - i] == _pattern)
                return (_size - 1) - i;
        return -1;
    }

    template<typename T>
    TS::String<T> TS::String<T>::Replace(const T original, const T _new) const
    {
        String result;
        result.Resize(_size);
        for (unsigned i = 0; i < _size; ++i)
        {
            if (_data[i] == original)
                result[i] = _new;
            else
                result[i] = _data[i];
        }

        return result;
    }

    template<typename T>
    TS::String<T> TS::String<T>::Replace(unsigned start, unsigned size, const T* str) const
    {
        if (size > _size)
            throw;

        String<T> result;
        result.Reserve(_size * 2);
        result.Resize(1);

        if (start == 0)
            result.AddRange(str)
                  .AddRange(SubString(size));
        else
            result.AddRange(SubString(0, start))
                  .AddRange(str)
                  .AddRange(SubString(start + size));

        return result;
    }

    template<typename T>
    TS::String<T> TS::String<T>::Replace(const T* original, const T* _new) const
    {
        const int sz = StringLength(original) - 1;

        int _index = Find(original);

        if (_index == -1)
            return *this;

        String<T> result = _data;
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
    size_t TS::String<T>::StringLength(const T* str)
    {
        size_t sz = 0;
        while (str[sz++] != '\0')
        {
            //! empty loop
        }
        return sz;
    }

    template <typename T>
    String<T>::~String()
    {
        if (_data != nullptr)
            TS_DELETE(_data);
    }

    template<typename T>
    bool TS::String<T>::operator==(const String<T>& string) const
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

    template<typename T>
    bool TS::String<T>::operator!=(const String<T>& string) const
    {
        return !(*this == string);
    }

    template<typename T>
    bool TS::String<T>::operator==(const T* string) const
    {
        size_t sz = StringLength(string);
        if (_size != StringLength(string))
            return false;

        for (size_t i = 0; i < _size; ++i)
        {
            if (_data[i] != string[i])
                return false;
        }
        return true;
    }

    template<typename T>
    bool TS::String<T>::operator!=(const T* string) const
    {
        return !(*this == string);
    }

    template<typename T>
    inline bool TS::String<T>::IsNullOrEmpty() const
    {
        return _data == nullptr || _size == 0 || _data[0] == 0;
    }


    template <typename T>
    String<T>& String<T>::Reserve(size_t capacity)
    {
        if (_capacity < capacity)
        {
            _capacity = capacity;
            T* old_data = _data;
            _data = TS_NEWARRAY(T, _capacity);

            if (old_data != nullptr)
            {
                copy_internal(0, _size, old_data);
                TS_DELETE(old_data);
            }
        }
        return *this;
    }

    template <typename T>
    String<T>& String<T>::Clear()
    {
        Resize(0);
        return *this;
    }

    template <typename T>
    String<T>& String<T>::Resize(size_t size)
    {
        Reserve(size);
        _size = size;

        if (_size != 0)
            _data[_size - 1] = 0;

        return *this;
    }

    template <typename T>
    String<T>& String<T>::AddRange(const T* string)
    {
        const size_t sz = StringLength(string);
       
        if (sz == 0)
            return *this;
        
        // ! èIí[Çë}ì¸Ç∑ÇÈ
        if (_size == 0)
            Resize(1);

        if (_size + sz <= _capacity)
        {
            copy_internal(_size - 1, sz, string);
            _size += sz - 1;
        }
        else
        {
            size_t newCapacity = _capacity == 0 ? 1 : _capacity;

            while (newCapacity < _size + sz)
                newCapacity *= 2;
            Reserve(newCapacity);
            copy_internal(_size - 1, sz, string);
            _size += sz - 1;
        }

        return *this;
    }

    template<>
    inline TS::StringA TS::StringA::Format(const char* pcFormat, ...)
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
    inline TS::StringW TS::StringW::Format(const wchar_t* pcFormat, ...)
    {
        va_list valist;

        va_start(valist, pcFormat);
        const size_t _maxBuffer = 65535;
        static wchar_t table[_maxBuffer]{};

        vswprintf_s(table, _maxBuffer, pcFormat, valist);
        va_end(valist);
        return table;
    }

    template <typename T>
    String<T>& String<T>::copy_internal(size_t start, size_t count, const T* buffer)
    {
        for (size_t i = 0; i < count; ++i)
            _data[start + i] = buffer[i];
        return *this;
    }

    template <typename T>
    String<T>& String<T>::copy_all_internal(const T* data, size_t size)
    {
        const size_t sz = size;
        Reserve(sz * 2);
        Resize(sz);
        copy_internal(0, sz, data);
        return *this;
    }
}
