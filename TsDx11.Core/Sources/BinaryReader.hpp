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
        memcpy_s(&data, sz, &_binary[_current], sz);
        _current += static_cast<unsigned>(sz);
        return data;
    }

    template <typename T>
    MemoryManagedArray<T> BinaryReader::ReadArray(unsigned dataCount)
    {
        const size_t sz = sizeof(T) * dataCount;
        MemoryManagedArray<T> datas(dataCount);
        memcpy_s(datas, sz, &_binary[_current], sz);
        _current += static_cast<unsigned>(sz);
        return datas;
    }

    inline TsString BinaryReader::ReadString()
    {
        unsigned counter = _current;
        while (counter < _binary.Length() && _binary[counter++] != '\0')
        {
        }

        TsString string(counter);
        for (unsigned i = 0; i < counter; ++i)
            string[i] = _binary[_current++];
        return string;
    }

    inline BinaryReader& BinaryReader::Seek(unsigned offset)
    {
        _current = offset;
        return *this;
    }

    inline BinaryReader BinaryReader::MakeSubReaderFromCurrent(unsigned current_to_offset)
    {
        return { Binary(&_binary[_current], current_to_offset) };
    }

    inline BinaryReader BinaryReader::MakeSubReaderFromHead(unsigned head_to_offeset, unsigned count)
    {
        if (count == 0)
            count = static_cast<unsigned>(_binary.Length()) - head_to_offeset;

        return { Binary(&_binary[head_to_offeset], count) };
    }
}

