#include "pch.h"
#include "FileReader.h"
namespace TS
{
    FileReader FileReader::ReadLine()
    {
        unsigned start = _current;

        if (_binary[_current] == '\n' ||
            _binary[_current] == '\r' )
        {
            start++;
        }

        while (true)
        {
            if (Eof()) break;
            _current++;

            if (_binary[_current] == '\n' ||
                _binary[_current] == '\r' )
            {
                _current++;
                break;
            }
        }

        return Binary(&_binary[start], _current - start);
    }

    Array<unsigned char> FileReader::ToArray() const
    {
        return Binary(&_binary[_current], _binary.Length() - _current);
    }

    unsigned FileReader::Current() const
    {
        return _current;
    }

    bool FileReader::Contain(const char* tag) const
    {
        return Find(tag) != -1;
    }

    int FileReader::Find(const char* tag) const
    {
        unsigned index = 0;
        auto binary = ToArray();

        return BoyerMooreStringSearch(reinterpret_cast<const char *>(&binary.Data()[Current()]),
            binary.Length() - Current(),
            tag,
            StringA::StringLength(tag));
    }
}

