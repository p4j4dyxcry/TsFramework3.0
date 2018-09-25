#pragma once

namespace TS
{
    class BitStream
    {
    public:
        BitStream(const Binary& binary):_binary(binary),_currentBit(0),_currentByte(0){}

        unsigned Read()
        {
            const unsigned byte = _binary[_currentByte];
            const unsigned maskedByte = byte & (0x01 << _currentBit);

            return (maskedByte == 0) ? 0 : 1;
        }

        unsigned ReadBits(unsigned bits_length)
        {
            unsigned bit = 0;
            for (unsigned i = 0; i < bits_length; ++i)
            {
                bit |= (Read() << i);
            }
            return bit;
        }


        BitStream& SeekNextByte()
        {
            if(_currentBit != 0)
            {
                _currentBit = 0;
                ++_currentByte;
            }
            return *this;
        }
        BitStream& seek_next()
        {
            ++_currentBit;
            if (_currentBit == 8)
            {
                _currentBit = 0;
                ++_currentByte;
            }
            return *this;
        }

        bool Eof() const
        {
            return _currentByte >= _binary.Length();
        }

    private:
        Binary _binary;
        size_t _currentBit;
        size_t _currentByte;
    };
}