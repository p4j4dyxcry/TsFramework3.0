#pragma once

namespace TS
{
    inline BinaryReader::BinaryReader(const Binary& binary) : _current(0), _binary(binary)
    {
    }
    
    template <typename T>
    T BinaryReader::Read()
    {
        T data;
        const size_t sz = sizeof(T);
        memcpy_s(&data, sz, &_binary.Data()[_current], sz);
        _current += static_cast<unsigned>(sz);
        return data;
    }

    template <typename T>
    ManagedArray<T> BinaryReader::ReadArray(unsigned dataCount)
    {
        const size_t sz = sizeof(T) * dataCount;
        ManagedArray<T> datas(dataCount);
        memcpy_s(datas, sz, &_binary.Data()[_current], sz);
        _current += static_cast<unsigned>(sz);
        return datas;
    }

    inline StringA BinaryReader::ReadString()
    {
        unsigned counter = _current;
        while (counter < _binary.Length() && _binary[counter++] != '\0')
        {
        }

        StringA string;
        for (unsigned i = 0; i < counter; ++i)
            string[i] += _binary[_current++];
        return string;
    }

    inline BinaryReader& BinaryReader::Seek(unsigned offset)
    {
        _current = offset;
        return *this;
    }
}

