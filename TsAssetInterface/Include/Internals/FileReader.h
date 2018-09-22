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
			
			while (true)
			{
				if (Eof())
					break;

				_current++;
			}

			return FileReader(Binary(&_binary[start], _current - start));
		}
		Array<unsigned char> ToArray()const
		{
			return Binary(&_binary[_current], _binary.Length() - _current);
		}

	};
}

