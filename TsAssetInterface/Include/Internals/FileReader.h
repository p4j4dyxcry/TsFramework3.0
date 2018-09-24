#pragma once

namespace TS 
{
	class FileReader : public BinaryReader
	{
	public:
		FileReader(const Binary& binary):BinaryReader(binary){}

		FileReader ReadLine()
		{
			unsigned start = _current;

			if (_binary[_current] == '\n' ||
				_binary[_current] == '\r' ||
				_binary[_current] == '\r\n')
			{
				start++;
			}
			
			while (true)
			{
				if (Eof()) break;
				_current++;

				if (_binary[_current] == '\n' ||
					_binary[_current] == '\r' ||
					_binary[_current] == '\r\n')
				{
					_current++; break; 
				}
			}

			return FileReader(Binary(&_binary[start], _current - start));
		}
		Array<unsigned char> ToArray()const
		{
			return Binary(&_binary[_current], _binary.Length() - _current);
		}

		unsigned Current()const
		{
			return _current;
		}

	};
}

