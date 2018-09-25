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
}

