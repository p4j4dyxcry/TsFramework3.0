#pragma once

namespace TS
{
    inline BinaryReader::BinaryReader(const Binary& binary) : _current(0), _binary(binary)
    {
    }
    
    template <typename T>
    T BinaryReader::Read()
    {
        const size_t sz = sizeof(T);
        T* ptr = reinterpret_cast<T*>(&_binary.Data()[_current]);
        _current += static_cast<unsigned>(sz);
        return *ptr;
    }

    template <typename T>
    Collection<T> BinaryReader::ReadArray(unsigned dataCount)
    {
        const size_t sz = sizeof(T) * dataCount;
        Collection<T> datas(dataCount);
        memcpy_s(datas, sz, &_binary.Data()[_current], sz);
        _current += static_cast<unsigned>(sz);
        return datas;
    }

    template <typename T>
    BinaryReader& BinaryReader::ReadArray(T* output, unsigned dataCount)
    {
        const size_t sz = sizeof(T) * dataCount;
        memcpy_s(output, sz, &_binary.Data()[_current], sz);
        _current += static_cast<unsigned>(sz);
        return *this;
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

    inline BinaryReader& BinaryReader::Skip(unsigned offset)
    {
        _current += offset;
        return *this;
    }
}

